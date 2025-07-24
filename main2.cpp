#include "../include/TSParser.h"
#include "../include/PatPmtParser.h"
#include "PacketPrinter.h"
#include "PidMapper.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ts_analyzer <file.ts>\n";
        return 1;
    }
    const char* tsPath = argv[1];

    // 1) parsiraj sve pakete + grupe po PID-u
    TSParser parser;
    auto packets  = parser.parseTransportStream(tsPath);
    auto grouped  = parser.groupPacketsByPID(packets);

    std::unordered_map<uint16_t,std::string> noNames;
    parser.printGroupedPackets(grouped, noNames);      // može i da se izbaci

    // 2) PAT/PMT ➜ mapiranje PID-ova po servisu
    TsPaketParser patPmt;
    if (!patPmt.parseFromGroupedPackets(grouped)) {
        std::cerr << "PAT/PMT parsing failed!\n";
        return 1;
    }
    PidMapper::printByService(patPmt);

    // 3) HEX-dump svih paketa
    std::ifstream in(tsPath, std::ios::binary);
    if (!in) { std::cerr << "Cannot open file!\n"; return 1; }

    constexpr int SZ = 188;
    uint8_t buf[SZ];
    for (int idx = 0; in.read(reinterpret_cast<char*>(buf), SZ); ++idx) {
        if (idx < static_cast<int>(packets.size()))
            PacketPrinter::printPacketHex(packets[idx], buf, SZ, idx);
    }
    return 0;
}
