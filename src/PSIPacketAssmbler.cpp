#include "PSIPacketAssembler.h"

void PSIPacketAssembler::reset() {
    buffer.clear();
    expected_section_length = 0;
    collecting = false;
    last_cc = -1;
}

std::vector<std::vector<uint8_t>> PSIPacketAssembler::process(const std::vector<TSPacket>& packets) {
    std::vector<std::vector<uint8_t>> completed_sections;

    for (size_t i = 0; i < packets.size(); i++) {
        const TSPacket& current_packet = packets[i];

        // if (current_packet.header.bf.sync_byte != 0x47) continue;
        if (current_packet.header.bf.transport_error_indicator) continue;

        const uint8_t* payload = current_packet.payload;
        const uint8_t* payload_end = payload + current_packet.payload_length;
        // if (!payload) continue;

        int8_t continuity_counter = current_packet.header.bf.continuity_counter;
        bool discontinuity = (this->last_cc != -1 && ((this->last_cc + 1) & 0b1111) != continuity_counter);
        this->last_cc = continuity_counter;

        if (discontinuity) {
            this->reset();
            continue;
        }

        if (current_packet.header.bf.payload_unit_start_indicator) {
            this->buffer.clear();

            if (static_cast<size_t>(payload[0] + 1) > current_packet.payload_length) {
                this->reset();
                continue;
            }

            const uint8_t* section_start = payload + payload[0] + 1; // Account for pointer_field 

            size_t bytes_left_in_payload = current_packet.payload_length - (section_start - payload);
            
            if(bytes_left_in_payload < 3) {
                this->reset();
                continue;
            }

            uint16_t section_length_with_crc = ((section_start[1] & 0b1111) << 8) | section_start[2];
            this->expected_section_length = 3 + section_length_with_crc;

            const size_t bytes_to_copy = this->expected_section_length <= bytes_left_in_payload ? 
                this->expected_section_length : bytes_left_in_payload; 

            this->buffer.insert(
                buffer.end(),
                section_start,
                section_start + bytes_to_copy
            );

            this->collecting = true;

            if (this->buffer.size() >= expected_section_length) {
                completed_sections.push_back(std::move(buffer));
                this->reset();

                // There could still be another section here, after the first one
                uint8_t* next_section = const_cast<uint8_t*>(section_start) + bytes_to_copy;
                while (true) {
                    if (next_section + 3 > payload_end) break;   // There is no space left for a new section
                    if (*next_section == 0xFF) break;           // Padding; there is nothing else here

                    // There is a valid section after the previous one
                    std::cout << "There are multiple valid sections in this packet." << std::endl;
                    uint16_t next_section_length_with_crc = ((next_section[1] & 0b1111) << 8) | next_section[2];
                    uint16_t total_next_section_length = 3 + next_section_length_with_crc;

                    // The next one is in its entirety
                    if (next_section + total_next_section_length < payload_end) {
                        completed_sections.emplace_back(
                            next_section,
                            next_section + total_next_section_length
                        );

                        next_section += total_next_section_length;
                    }
                    else { // The next one is fragmented
                        this->buffer.insert(this->buffer.end(), next_section, const_cast<uint8_t*>(payload_end));
                        this->expected_section_length = total_next_section_length;
                        this->collecting = true;
                        break;
                    }
                }
            }
        }
        else if (collecting) {
            const uint8_t* section_start = payload;
            size_t bytes_left_in_payload = current_packet.payload_length - (section_start - payload);

            const size_t bytes_to_copy = (this->expected_section_length - this->buffer.size()) <= bytes_left_in_payload ?
                this->expected_section_length - this->buffer.size() : bytes_left_in_payload;

            this->buffer.insert(buffer.end(), section_start, section_start + bytes_to_copy);

            if (this->buffer.size() >= this->expected_section_length) {
                completed_sections.push_back(std::move(this->buffer));
                this->reset();
            }
        }
    }

    return completed_sections;
}