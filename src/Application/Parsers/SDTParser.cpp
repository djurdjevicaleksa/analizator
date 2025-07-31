#include <vector>
#include <cassert>

#include "PSIPacketAssembler.h"
#include "SDTParser.h" 

std::vector<ServiceDescriptionSection> SDT::parse(std::vector<TSPacket>& ts_packets) {

    PSIPacketAssembler assembler;
    std::vector<std::vector<uint8_t>> completed_sections = assembler.process(ts_packets);

    std::vector<ServiceDescriptionSection> sdt_tables;

    for (size_t i = 0; i < completed_sections.size(); i++) {

        std::vector<uint8_t>& byte_array = completed_sections[i];
        ServiceDescriptionSection sdt;

        // Extract the header
        sdt.header.table_id = byte_array[0];
        sdt.header.section_syntax_indicator = (byte_array[1] >> 7) & 0b1;
        sdt.header.section_length = ((byte_array[1] & 0b1111) << 8) | byte_array[2];
        sdt.header.transport_stream_id = (byte_array[3] << 8) | byte_array[4];
        sdt.header.version_number = (byte_array[5] >> 1) & 0b11111;
        sdt.header.current_next_indicator = byte_array[5] & 0b1;
        sdt.header.section_number = byte_array[6];
        sdt.header.last_section_number = byte_array[7];
        sdt.header.original_network_id = (byte_array[8] << 8) | byte_array[9];

        uint8_t* service_start = &byte_array[11];
        uint8_t* service_end = &byte_array[byte_array.size() - 4];

        while (service_start < service_end) {
            ServiceDescriptionSection::Service service;

            service.service_id = (service_start[0] << 8) | service_start[1];
            service.eit_schedule_flag = (service_start[2] >> 1) & 0b1;
            service.eit_present_following_flag = service_start[2] & 0b1;
            service.running_status = (service_start[3] >> 5) & 0b111;
            service.free_ca_mode = (service_start[3] >> 4) & 0b1;
            service.descriptors_loop_length = ((service_start[3] & 0b1111) << 8) | service_start[4];

            uint8_t* iter = &service_start[5];
            uint8_t* descriptor_loop_end = iter + service.descriptors_loop_length;

            assert(iter < descriptor_loop_end);

            while (iter + 1 < descriptor_loop_end) {
                if (iter + 2 + *(iter + 1) > descriptor_loop_end) {
                    std::cerr << "[SDT PARSE] Malformed service descriptor." << std::endl;
                    return {}; // TODO This wont work properly
                }

                service.descriptors.emplace_back(*iter, *(iter + 1), iter + 2);
                iter += 2 + *(iter + 1);
            }
            sdt.services.push_back(std::move(service));
            service_start = descriptor_loop_end;
        }

        sdt.crc = static_cast<uint32_t>(service_end[0] << 24)
                | static_cast<uint32_t>(service_end[1] << 16)
                | static_cast<uint32_t>(service_end[2] << 8)
                | static_cast<uint32_t>(service_end[3]);
        sdt_tables.push_back(std::move(sdt));
    }
    return sdt_tables;
}