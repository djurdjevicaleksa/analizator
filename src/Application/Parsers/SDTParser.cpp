#include <vector>
#include <unordered_map>
#include <iostream>

#include "src/Application/Parsers/PSIPacketAssembler.h"
#include "src/Application/Parsers/SDTParser.h" 

void SDT::parse(const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_ts_packets, std::vector<ServiceDescriptionSection>& out) {

    std::vector<std::vector<std::uint8_t>> completed_sections;
    try {
        PSI::process(grouped_ts_packets.at(SDT::PID), completed_sections);
    }
    catch (std::out_of_range& e) { return; }

    for (std::size_t i = 0; i < completed_sections.size(); i++) {
        ServiceDescriptionSection sdt(&completed_sections[i][0]);

        if (sdt.long_header.has_value()) {
            if (out.size() > 0) {
                if (out.rbegin()->long_header.has_value()) {
                    if (sdt.long_header.value().version_number == out.rbegin()->long_header.value().version_number) {
                        // Should do a PCR check here; currently if either the current or past SDT
                        // don't have the extended header, I have no clue whether or not the version changed
                        continue;
                    }
                }
            }
        }
        out.push_back(std::move(sdt));
    }
}