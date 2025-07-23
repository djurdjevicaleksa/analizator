#include "TSParser.h"
#include "NITParser.h"

int main(int argc, char** argv) {
    TSParser ts_parser;

    std::vector<TSPacket> ts_packets = ts_parser.parseTransportStream(argv[1]);
    std::unordered_map<uint16_t, std::vector<TSPacket>> grouped_packets = ts_parser.groupPacketsByPID(ts_packets);

    // ts_parser.printGroupedPackets(grouped_packets);

    std::vector<NetworkInformationSection> nit_tables = NIT::parse(grouped_packets.at(0x0010));
    nit_tables[0].print();

    return 0;
}