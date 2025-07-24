#include "../include/PatPmtParser.h"
#include "../include/TSPacket.h"
#include "../include/TSParser.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>

#define INPUT_FILE "/home/janap/ts/25Services.ts"

int main(int argc, char** argv) {
    std::ifstream infile(INPUT_FILE, std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << INPUT_FILE << "\n";
        return 1;
    }

    // Mapa za prikaz prilagođenih imena PID-ova
    std::unordered_map<uint16_t, std::string> custom_pid_names;

    // 1. Parsiranje TS paketa
    TSParser ts_parser;
    std::vector<TSPacket> ts_packets = ts_parser.parseTransportStream(INPUT_FILE);
    auto grouped_packets = ts_parser.groupPacketsByPID(ts_packets);

    // 2. Parsiranje PAT i PMT
    TsPaketParser pat_pmt_parser;
    pat_pmt_parser.parseFromGroupedPackets(grouped_packets);

    // 3. Popunjavanje custom_pid_names na osnovu PMT-a
    for (const auto& program : pat_pmt_parser.getPrograms()) {
        custom_pid_names[program.pmt_pid] = "PMT (Program Map Table)";

        for (const auto& stream : program.streams) {
            std::string type_str;
            switch (stream.type) {
                case StreamType::VIDEO:    type_str = "Video Stream"; break;
                case StreamType::AUDIO:    type_str = "Audio Stream"; break;
                case StreamType::SUBTITLE: type_str = "Subtitle Stream"; break;
                case StreamType::DATA:     type_str = "Data Stream"; break;
                default:                   type_str = "Unknown Stream"; break;
            }

            custom_pid_names[stream.pid] = type_str + " (" + stream.codec + ")";
        }
    }

    // 4. Ispis grupa paketa sa nazivima PID-ova
    ts_parser.printGroupedPackets(grouped_packets, custom_pid_names);

    // 5. Ispis dodatnih informacija iz PAT/PMT
    pat_pmt_parser.printInfo();

    return 0;
}
