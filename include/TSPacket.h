#ifndef _TSPACKET_H
#define _TSPACKET_H

#include <cstdint>
#include <memory>
#include <iostream>
#include <iomanip>

struct TSPacket {
    struct header_t {
        unsigned sync_byte: 8;
        unsigned transport_error_indicator: 1;
        unsigned payload_unit_start_indicator: 1;
        unsigned transport_priority: 1;
        unsigned pid: 13;
        unsigned transport_scrambling_control: 2;
        unsigned adaptation_field_control: 2;
        unsigned continuity_counter: 4;
    };

    union header {
        header_t bf;
        uint32_t raw;
    };

    header header;

    uint8_t* adaptation_field = nullptr;
    size_t adaptation_field_length = 0;

    uint8_t* payload = nullptr;
    size_t payload_length = 0;

    void print() {
        std::cout << "==================================================================================================" << std::endl;
        std::cout << " Sync byte: " << std::hex << header.bf.sync_byte << std::endl;
        std::cout << " Transport error indicator: " << header.bf.transport_error_indicator << std::endl;
        std::cout << " Payload unit start indicator: "  << header.bf.payload_unit_start_indicator << std::endl;
        std::cout << " Transport priority: " << header.bf.transport_priority << std::endl;
        std::cout << " PID=0x" << std::hex << std::setw(4) << std::setfill('0') << header.bf.pid << std::endl;
        std::cout << " Transport scrambling control: " << header.bf.transport_scrambling_control << std::endl;
        std::cout << " Adaptation field control: " << header.bf.adaptation_field_control << std::endl;
        std::cout << " Continuity counter: " << std::dec << header.bf.continuity_counter << std::endl;

        if (adaptation_field && adaptation_field_length > 0) {
            std::cout << " Adaptation field (" << std::dec << adaptation_field_length << " bytes):\n ";
            for (size_t i = 0; i < adaptation_field_length; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(adaptation_field[i]) << " ";
                if ((i + 1) % 16 == 0) std::cout << "\n ";
            }
            std::cout << std::dec << std::endl;
        }
    
        if (payload && payload_length > 0) {
            std::cout << " Payload (" <<  std::dec << payload_length << " bytes):\n ";
            for (size_t i = 0; i < payload_length; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(payload[i]) << " ";
                if ((i + 1) % 16 == 0) std::cout << "\n ";
            }
            std::cout << std::dec << std::endl;
        }
    
        std::cout << "==================================================================================================" << std::endl;
    }
};

#endif // _TSPACKET_H