#include <vector>
#include <unordered_map>
#include <iostream>

#include "src/Application/Parsers/PSIPacketAssembler.h"
#include "src/Application/Parsers/NITParser.h"

void NIT::parse(const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_ts_packets, std::vector<NetworkInformationSection>& out) {

    std::vector<std::vector<std::uint8_t>> completed_sections;
    try {
        PSI::process(grouped_ts_packets.at(NIT::PID), completed_sections);
    }
    catch (std::out_of_range& e) { return; }

    for (std::size_t i = 0; i < completed_sections.size(); i++) {
        NetworkInformationSection nit(&completed_sections[i][0]);
        
        if(nit.long_header.has_value()) {
            if (out.size() > 0) {
                if (out.rbegin()->long_header.has_value()) {
                    if (nit.long_header.value().version_number == out.rbegin()->long_header.value().version_number) {
                        // Should do a PCR check here; currently if either the current or past NIT
                        // don't have the extended header, I have no clue whether or not the version changed
                        continue;
                    }
                }
            }
        }
        out.push_back(std::move(nit));
    }
}