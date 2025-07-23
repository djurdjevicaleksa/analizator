#include "../include/PatPmtParser.h"
#include "../include/TSPacket.h"
#include "../include/TSParser.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

#define INPUT_FILE "/home/milan-strbac/Desktop/DTV/analizator/ts/DVB-T2_multiplex.ts"

int main(int argc, char** argv) {
    std::ifstream infile(INPUT_FILE, std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << INPUT_FILE << "\n";
        return 1;
    }

    // 1. Parsiranje TS paketa
    TSParser ts_parser;
    std::vector<TSPacket> ts_packets = ts_parser.parseTransportStream(INPUT_FILE);
    auto grouped_packets = ts_parser.groupPacketsByPID(ts_packets);
    ts_parser.printGroupedPackets(grouped_packets);

    // 2. Parsiranje PAT koristeći TsPaketParser
    TsPaketParser pat_pmt_parser;

    pat_pmt_parser.parseFromGroupedPackets(grouped_packets);
    pat_pmt_parser.printInfo();

    return 0;
}
