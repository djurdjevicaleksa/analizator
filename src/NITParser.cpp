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
        NetworkInformationSection nis;

        // Extract the header
        nis.header.table_id = byte_array[0];
        nis.header.section_syntax_indicator = (byte_array[1] >> 7) & 0b1;
        nis.header.section_length = ((byte_array[1] & 0b1111) << 8) | byte_array[2];
        nis.header.network_id = (byte_array[3] << 8) | byte_array[4];
        nis.header.version_number = (byte_array[5] >> 1) & 0b11111;
        nis.header.current_next_indicator = byte_array[5] & 0b1;
        nis.header.section_number = byte_array[6];
        nis.header.last_section_number = byte_array[7];
        nis.header.network_descriptors_length = ((byte_array[8] & 0b1111) << 8) | byte_array[9];

        // Boudary control
        uint8_t* section_start = &byte_array[3];
        uint8_t* section_end = section_start + nis.header.section_length;
        uint16_t total_section_length = 3 + nis.header.section_length;

        assert(total_section_length == byte_array.size());

        // Loop through network descriptors
        uint8_t* iter = &byte_array[10];
        uint8_t* descriptor_loop_end = iter + nis.header.network_descriptors_length;
        
        assert(iter < descriptor_loop_end);
        assert(descriptor_loop_end <= section_end);
        
        while(iter + 1 < descriptor_loop_end) {
            if (iter + 2 + *(iter + 1) > descriptor_loop_end) {
                std::cerr << "[NIT PARSE] Malformed network descriptor.\n";
                return {};
            }

            nis.network_descriptors.emplace_back(*iter, *(iter + 1), iter + 2);
            iter += 2 + *(iter + 1);
        }

        // Loop through TS descriptors
        uint8_t* window = descriptor_loop_end;
        nis.header.transport_stream_loop_length = ((window[0] & 0b1111) << 8) | window[1];

        iter = &window[2];
        uint8_t* ts_loop_end = iter + nis.header.transport_stream_loop_length;
        assert(ts_loop_end <= section_end);

        while (iter < ts_loop_end) {
            uint16_t tsid = (iter[0] << 8) | iter[1];
            uint16_t onid = (iter[2] << 8) | iter[3];
            uint16_t transport_descriptors_length = ((iter[4] & 0b1111) << 8) | iter[5];
            std::vector<NetworkInformationSection::Descriptor> descriptors;

            uint8_t* inner_iter = &iter[6];
            uint8_t* inner_loop_end = inner_iter + transport_descriptors_length;

            while(inner_iter < inner_loop_end) {
                descriptors.emplace_back(*inner_iter, *(inner_iter + 1), inner_iter + 2);
                inner_iter += 2 + *(inner_iter + 1);
            }

            nis.ts_loop_entries.emplace_back(tsid, onid, transport_descriptors_length, descriptors);

            iter = inner_loop_end;
        }

        nis.crc = static_cast<uint32_t>(ts_loop_end[0] << 24)
                | static_cast<uint32_t>(ts_loop_end[1] << 16)
                | static_cast<uint32_t>(ts_loop_end[2] << 8)
                | static_cast<uint32_t>(ts_loop_end[3]);
        
        nit_tables.push_back(std::move(nis));
    }
    return nit_tables;
}