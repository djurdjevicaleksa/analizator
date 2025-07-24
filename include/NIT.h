#ifndef _NIT_H
#define _NIT_H

#include <cstdint>
#include <vector>

#include "TSPacket.h"
#include "Descriptor.h"

struct NetworkInformationSection {
    struct PacketHeader {
        uint8_t table_id;
        bool section_syntax_indicator;
        uint16_t section_length;
        uint16_t network_id;
        uint8_t version_number;
        bool current_next_indicator;
        uint8_t section_number;
        uint8_t last_section_number;
        uint16_t network_descriptors_length;
        uint16_t transport_stream_loop_length;

        void print() const;
    };
    
    struct TSLoopEntry {
        uint16_t tsid;
        uint16_t onid;
        uint16_t length;
        std::vector<Descriptor> descriptors;

        TSLoopEntry(uint16_t ts, uint16_t on, uint16_t len, std::vector<Descriptor>& desc) : tsid(ts), onid(on), length(len), descriptors(std::move(desc)) {}
        void print() const;
    };
    
    PacketHeader header;    
    std::vector<Descriptor> network_descriptors;
    std::vector<TSLoopEntry> ts_loop_entries;
    uint32_t crc;

    void print() const;
    static std::string getDescriptorTypeFromTag(uint8_t);
};

#endif // _NIT_H