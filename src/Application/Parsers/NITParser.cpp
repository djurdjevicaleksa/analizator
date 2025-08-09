#include <vector>
#include <cassert>

#include "PSIPacketAssembler.h"
#include "NITParser.h"

void NIT::parse(const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_ts_packets, std::vector<NetworkInformationSection>& out) {

    std::vector<std::vector<std::uint8_t>> completed_sections;
    PSI::process(grouped_ts_packets.at(NIT::PID), completed_sections);

    for (std::size_t i = 0; i < completed_sections.size(); i++) {
        out.emplace_back(&completed_sections[i][0]);
    }
}