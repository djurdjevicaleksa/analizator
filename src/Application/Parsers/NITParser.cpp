#include <vector>
#include <cassert>

#include "PSIPacketAssembler.h"
#include "NITParser.h"

std::vector<NetworkInformationSection> NIT::parse(std::vector<TSPacket>& ts_packets) {

    PSIPacketAssembler assembler;
    std::vector<std::vector<uint8_t>> completed_sections = assembler.process(ts_packets);

    std::vector<NetworkInformationSection> nit_tables;

    for (size_t i = 0; i < completed_sections.size(); i++) {

        std::vector<uint8_t>& byte_array = completed_sections[i];
        nit_tables.emplace_back(&byte_array[0]);
    }
    
    return nit_tables;
}