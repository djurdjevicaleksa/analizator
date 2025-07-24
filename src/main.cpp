#include "../include/TSParser.h"
#include "../include/PatPmtParser.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

void printFullPacketHex(const TSPacket& pkt, const uint8_t* full_data, size_t size, int index) {
    std::cout << "\nPacket " << index << " (188 bytes):\n";
    for (size_t i = 0; i < size; ++i) {
        if (i % 16 == 0)
            std::cout << std::setw(4) << std::setfill('0') << std::hex << i << ": ";
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(full_data[i]) << " ";
        if ((i + 1) % 16 == 0) std::cout << "\n";
    }
    std::cout << std::dec << "\n";

    // Semantička interpretacija TS headera
    std::cout << "  Sync Byte: 0x" << std::hex << ((full_data[0] & 0xFF)) << "\n";
    std::cout << "  Transport Error Indicator: " << ((full_data[1] & 0x80) >> 7) << "\n";
    std::cout << "  Payload Unit Start Indicator: " << ((full_data[1] & 0x40) >> 6) << "\n";
    std::cout << "  Transport Priority: " << ((full_data[1] & 0x20) >> 5) << "\n";
    std::cout << "  PID: 0x" << std::hex << (((full_data[1] & 0x1F) << 8) | full_data[2]) << "\n";
    std::cout << "  Scrambling Control: " << ((full_data[3] & 0xC0) >> 6) << "\n";
    std::cout << "  Adaptation Field Control: " << ((full_data[3] & 0x30) >> 4) << "\n";
    std::cout << "  Continuity Counter: " << ((full_data[3] & 0x0F)) << "\n";
    std::cout << "--------------------------------------------------\n";
}

std::string getStreamTypeCategory(uint8_t stream_type) {
    switch (stream_type) {
        case 0x01: case 0x02: case 0x1B: case 0x24: return "video";
        case 0x03: case 0x04: case 0x0F: return "audio";
        case 0x06: return "subtitles";
        case 0x05: return "private";
        case 0x0D: return "metadata";
        case 0x80: return "ecm";
        case 0x81: return "emm";
        default: return "unknown";
    }
}

bool isVideoOrAudio(uint8_t stream_type) {
    return stream_type == 0x01 || stream_type == 0x02 || stream_type == 0x1B || stream_type == 0x24 ||
           stream_type == 0x03 || stream_type == 0x04 || stream_type == 0x0F;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./test <input.ts>\n";
        return 1;
    }

    TSParser parser;
    std::vector<TSPacket> packets = parser.parseTransportStream(argv[1]);
    auto grouped = parser.groupPacketsByPID(packets);
    parser.printGroupedPackets(grouped);
    
    TsPaketParser pat_pmt_parser;
    if (!pat_pmt_parser.parseFromGroupedPackets(grouped)) {
        std::cerr << "Failed to parse PAT/PMT tables.\n";
        return 1;
    }

    const auto& programs = pat_pmt_parser.getPrograms();
    std::cout << "\n=== PID Map by TV Service ===\n";
    for (const auto& prog : programs) {
        std::cout << "Program " << prog.program_number << " (PMT PID: 0x"
                  << std::hex << prog.pmt_pid << std::dec << "):\n";
        for (const auto& stream : prog.streams) {
            std::string pid_str;
            if (isVideoOrAudio(stream.stream_type_id)) {
                pid_str = std::to_string(stream.pid);  // decimal for video/audio
            } else {
                std::ostringstream oss;
                oss << "0x" << std::hex << stream.pid;
                pid_str = oss.str();  // hex for others
            }

            std::cout << "  PID: " << pid_str
                      << ", Codec: " << stream.codec
                      << ", Category: " << getStreamTypeCategory(stream.stream_type_id)
                      << "\n";
        }
        std::cout << std::endl;
    }

    // HEX prikaz celog paketa sa interpretacijom
    std::ifstream tsFile(argv[1], std::ios::binary);
    if (!tsFile.is_open()) {
        std::cerr << "Failed to open input TS file.\n";
        return 1;
    }

    const int PACKET_SIZE = 188;
    uint8_t buffer[PACKET_SIZE];
    int index = 0;
    while (tsFile.read(reinterpret_cast<char*>(buffer), PACKET_SIZE)) {
        if (index < packets.size()) {
            printFullPacketHex(packets[index], buffer, PACKET_SIZE, index);
        }
        index++;
    }

    return 0;
}
