#ifndef _TSPACKET_H
#define _TSPACKET_H

#include <cstdint>
#include <memory>

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

    // void print() {
    //     std::cout << "==================================================================================================" << std::endl;
    //     std::cout << " Transport error indicator: " << header.bf.transport_error_indicator << std::endl;
    //     std::cout << " Payload unit start indicator: "  << header.bf.payload_unit_start_indicator << std::endl;
    //     std::cout << " Transport priority: " << header.bf.transport_priority << std::endl;
    //     std::cout << " PID=0x" << std::hex << std::setw(4) << std::setfill('0') << header.bf.pid << std::endl;
    //     std::cout << " Transport scrambling control: " << header.bf.transport_scrambling_control << std::endl;
    //     std::cout << " Adaptation field control: " << header.bf.adaptation_field_control << std::endl;
    //     std::cout << " Continuity counter: " << header.bf.continuity_counter << std::endl;
    //     std::cout << "==================================================================================================" << std::endl;
    // }
};

#endif // _TSPACKET_H