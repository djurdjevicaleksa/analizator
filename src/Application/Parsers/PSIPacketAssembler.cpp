#include <vector>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <algorithm>

#include "PSIPacketAssembler.h"

void PSI::process(const std::vector<TSPacket*>& grouped_psi_packets, std::vector<std::vector<std::uint8_t>>& out_completed_sections) {

    std::vector<std::uint8_t> buffer;
    std::size_t expected_section_length = 0;
    bool collecting = false;
    std::int8_t last_cc = -1;

    auto reset = [&]() {
        buffer.clear();
        expected_section_length = 0;
        collecting = false;
        last_cc = -1;
    };

    for (std::size_t i = 0; i < grouped_psi_packets.size(); i++) {

        TSPacket& current_packet = *grouped_psi_packets[i];

        // Check continuity validity
        std::int8_t continuity_counter = current_packet.header.continuity_counter;
        bool discontinuity = (last_cc != -1) && (((last_cc + 1) & 0b1111) != continuity_counter) && current_packet.payload.has_value();
        if (current_packet.payload.has_value()) last_cc = continuity_counter;
        if (discontinuity) { reset(); continue; }

        if (!current_packet.payload.has_value()) continue;
        
        const std::uint8_t* payload_start = &current_packet.payload.value()[0];
        const std::uint8_t* payload_end = payload_start + current_packet.payload.value().size();
        

        if (current_packet.header.payload_unit_start_indicator) {
            
            const std::uint8_t* new_section_start = payload_start + *payload_start + 1;
            if (new_section_start >= payload_end) { reset(); continue; }
            
            // Complete the previous section, if it exists
            if (collecting) {
                buffer.insert(buffer.end(), payload_start + 1, new_section_start);
                out_completed_sections.push_back(buffer);
                // assert(buffer.size() == expected_section_length && "Expected section length does not equal the size of the buffer.");
                reset();
            }           
            
            last_cc = continuity_counter;
            buffer.clear();

            std::size_t bytes_left_in_payload = current_packet.payload.value().size() - (new_section_start - payload_start);
            if (bytes_left_in_payload < 3) {
                std::cerr << "Failed to assemble a PSI section: Section sliced at a wrong place." << std::endl;
                reset();
                continue;
            }
            
            std::uint16_t section_length_with_crc = ((new_section_start[1] & 0b1111) << 8) | new_section_start[2];
            expected_section_length = 3 + section_length_with_crc;

            const std::size_t bytes_to_copy = std::min(expected_section_length, bytes_left_in_payload);
            buffer.insert(buffer.end(), new_section_start, new_section_start + bytes_to_copy);
            collecting = true;

            if (buffer.size() >= expected_section_length) {
                out_completed_sections.push_back(std::move(buffer));
                reset();

                // There could still be more sections after the first one
                std::uint8_t* next_section_start = const_cast<std::uint8_t*>(new_section_start) + bytes_to_copy;
                while (true) {
                    if (next_section_start + 3 > payload_end) break;    // No space for a new section
                    if (*next_section_start == 0xFF) break;             // Stuffing bytes; there is nothing else here
                    
                    last_cc = continuity_counter;

                    // A valid section exists after the previous one
                    std::cout << "There are multiple valid sections in this packet." << std::endl;
                    std::uint16_t next_section_length_with_crc = ((next_section_start[1] & 0b11) << 8) | next_section_start[2];
                    std::uint16_t total_next_section_length = 3 + next_section_length_with_crc;


                    // If the next section is entirely inside the rest of the payload...
                    if (next_section_start + total_next_section_length < payload_end) {
                        out_completed_sections.emplace_back(
                            next_section_start,
                            next_section_start + total_next_section_length
                        );

                        next_section_start += total_next_section_length;
                    }
                    else {
                        buffer.insert(buffer.end(), next_section_start, const_cast<std::uint8_t*>(payload_end));
                        expected_section_length = total_next_section_length;
                        collecting = true;
                        break;
                    }              
                }
            }
        }
        else if (collecting) {
            const std::uint8_t* continued_section = payload_start;
            std::size_t bytes_left_in_payload = current_packet.payload.value().size();
            
            const std::size_t bytes_to_copy = std::min(expected_section_length - buffer.size(), bytes_left_in_payload);
            buffer.insert(buffer.end(), continued_section, continued_section + bytes_to_copy);

            if (buffer.size() >= expected_section_length) {
                out_completed_sections.push_back(std::move(buffer));
                reset();
            }
        }
    }
}