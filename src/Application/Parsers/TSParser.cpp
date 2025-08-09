#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "TSParser.h" 

// Packet sizes according to different standards
constexpr std::uint8_t TS_SIZE_STD = 188;
constexpr std::uint8_t TS_SIZE_M2TS = 192;
constexpr std::uint8_t TS_SIZE_DVBRS = 204;

constexpr std::uint8_t SYNC_BYTE = 0x47;

namespace TS {

    std::uint8_t getPacketSize(const char* input_file_name) {
        
        std::ifstream tsFile(input_file_name, std::ios::binary);
        
        if (!tsFile.is_open()) {
            std::cerr << "Failed to deduce TS packet size: Cannot open \"" << input_file_name << "\"" << std::endl;
            exit(1);
        }

        std::uint8_t packet_size;
        std::uint8_t buffer[256] = {0};

        tsFile.read(reinterpret_cast<char*>(buffer), sizeof(buffer));

        if (buffer[TS_SIZE_STD] == 0x47) {
            packet_size = TS_SIZE_STD;
        }
        else if (buffer[TS_SIZE_M2TS] == 0x47) {
            packet_size = TS_SIZE_M2TS;
        }
        else if (buffer[TS_SIZE_DVBRS] == 0x47) {
            packet_size = TS_SIZE_DVBRS;
        }
        else {
            std::cerr << "Failed to deduce TS packet size: Malformed .ts file" << std::endl;
            exit(1);
        }
        
        return packet_size;
    }


    void parseTransportStream(const char* input_file_name, std::vector<TSPacket>& out) {

        const std::uint8_t packet_size = getPacketSize(input_file_name);
        auto packet_buffer = std::make_unique<std::uint8_t[]>(packet_size);

        /*
            TODO: MEMORY MAP HERE
        */

        std::ifstream tsFile(input_file_name, std::ios::binary);
        if (!tsFile.is_open()) {
            std::cerr << "Failed to parse transport stream: Cannot open \"" << input_file_name << "\"" << std::endl;
            exit(1);
        }
        
        while (tsFile.read(reinterpret_cast<char*>(packet_buffer.get()), packet_size)) {
            if (packet_buffer[0] != SYNC_BYTE) 
                std::cerr << "Discarding a TS packet: Invalid sync byte value: " << packet_buffer[0] << std::endl;
            
            try {
                out.emplace_back(packet_buffer.get(), packet_size);
            }
            catch(std::runtime_error& e) {
                std::cerr << "Failed to construct a TS packet: " << e.what() << std::endl;
            }
        }

        out.shrink_to_fit();
    }


    std::string getNameByPID(std::uint16_t pid, const std::unordered_map<std::uint16_t, std::string>& user_defined_pid_names) {

        std::unordered_map<std::uint16_t, std::string> reserved_pid_names {
            {0x00, "PAT (Program Association Table)"},
            {0x01, "CAT (Conditional Access Table)"},
            {0x02, "TSDT (Transport Stream Description Table)"},
            // PIDs 0x3 - 0xF are reserved
            {0x10, "NIT (Network Information Table)"},
            {0x11, "SDT / BAT (Service Description Table) / (Bouquet Association Table)"},
            {0x12, "EIT (Event Information Table)"},
            {0x13, "RST (Running Status Table)"},
            {0x14, "TDT (Time and Date Table), TOT (Time Offset Table)"},
            {0x15, "Network Synchronization"},
            {0x16, "RNT (Rating Notification Table)"},
            {0x1C, "Inband Signalling"},
            {0x1D, "Measurement"},
            {0x1E, "DIT (Discontinuity Information Table)"},
            {0x1F, "SIT (Selection Information Table)"},
            {0x1FFF, "Null Packet"}
        };

        auto user_defined_name = user_defined_pid_names.find(pid);
        if (user_defined_name != user_defined_pid_names.end()) return user_defined_name->second;

        auto reserved_name = reserved_pid_names.find(pid);
        if (reserved_name != reserved_pid_names.end()) return reserved_name->second;

        return "Unknown PID";
    }

    void printPacketGroups(
        const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_packets,
        const std::unordered_map<std::uint16_t, std::string>& user_defined_pid_names
    ) {

        for (const auto& [pid, packet_group]: grouped_packets) {
            std::cout << "PID: 0x" << std::hex << pid
                      << " (Contains " << std::dec << packet_group.size() << " packets)"
                      << " - " << getNameByPID(pid, user_defined_pid_names) << std::endl;
        }
    }
} // namespace TS