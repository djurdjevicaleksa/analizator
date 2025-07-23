#ifndef _SDT_H
#define _SDT_H

#include <cstdint>
#include <vector>

#include "NIT.h"

#define SDT_PID 0x0011

struct ServiceDescriptionSection {
    struct PacketHeader {
        uint8_t table_id;
        bool section_syntax_indicator;
        uint16_t section_length;
        uint16_t transport_stream_id;
        uint8_t version_number;
        bool current_next_indicator;
        uint8_t section_number;
        uint8_t last_section_number;
        uint16_t original_network_id;

        void print() const;
    };

    struct Service {
        uint16_t service_id;
        bool eit_schedule_flag;
        bool eit_present_following_flag;
        uint8_t running_status;
        bool free_ca_mode;
        uint16_t descriptors_loop_length;
        std::vector<NetworkInformationSection::Descriptor> descriptors;

        void print() const;
    };
    
    PacketHeader header;
    std::vector<Service> services;
    uint32_t crc;

    void print() const;
};

#endif // _SDT_H

