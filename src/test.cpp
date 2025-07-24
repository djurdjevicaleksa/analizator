#include "Analizator.h"

// #include "TSParser.h"
// #include "NITParser.h"
// #include "SDTParser.h"

int main(int argc, char** argv) {
    // TSParser ts_parser;

    // std::vector<TSPacket> ts_packets = ts_parser.parseTransportStream(argv[1]);
    // std::unordered_map<uint16_t, std::vector<TSPacket>> grouped_packets = ts_parser.groupPacketsByPID(ts_packets);

    // ts_parser.printGroupedPackets(grouped_packets);

    // try {
    //     std::vector<NetworkInformationSection> nit_tables = NIT::parse(grouped_packets.at(NetworkInformationSection::NIT_PID));
    //     nit_tables[0].print();
    // }
    // catch (std::out_of_range& e) {
    //     std::cerr << "[TEST] The input file does not contain an NIT table." << std::endl;
    // }

    // try {
    //     std::vector<ServiceDescriptionSection> sdt_tables = SDT::parse(grouped_packets.at(ServiceDescriptionSection::SDT_PID));
    //     sdt_tables[0].print();
    // }
    // catch (std::out_of_range& e) {
    //     std::cerr << "[TEST] The input file does not contain an SDT table." << std::endl;
    // }

    Analizator analizator(argv[1]);
    analizator.printNITs();
    analizator.printSDTs();
    analizator.printProgramInfos();
    analizator.printGroupedPackets();

    return 0;
}