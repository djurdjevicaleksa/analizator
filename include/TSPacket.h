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

    void print() const {
        std::cout << "\033[34m";        
        std::cout << "========== HEADER ========== " << std::endl;
        std::cout << " Sync byte: " << std::hex << header.bf.sync_byte << std::endl;
        std::cout << " Transport error indicator: " << header.bf.transport_error_indicator << std::endl;
        std::cout << " Payload unit start indicator: "  << header.bf.payload_unit_start_indicator << std::endl;
        std::cout << " Transport priority: " << header.bf.transport_priority << std::endl;
        std::cout << " PID=0x" << std::hex << std::setw(4) << std::setfill('0') << header.bf.pid << std::endl;
        std::cout << " Transport scrambling control: " << header.bf.transport_scrambling_control << std::endl;
        std::cout << " Adaptation field control: " << header.bf.adaptation_field_control << std::endl;
        std::cout << " Continuity counter: " << std::dec << header.bf.continuity_counter << std::endl;
        std::cout << "==========/HEADER/========== " << std::endl;
        std::cout << "\033[0m" << std::endl;

        std::cout << "========== RAW PACKET ========== " << std::endl;
        uint8_t byte1 = (uint8_t)this->header.bf.sync_byte;
        uint8_t byte2 = ((uint8_t)this->header.bf.transport_error_indicator << 7)
        | ((uint8_t)this->header.bf.payload_unit_start_indicator << 6)
        | ((uint8_t)this->header.bf.transport_priority << 5)
        | ((this->header.bf.pid >> 8) & 0b11111);
        uint8_t byte3 = this->header.bf.pid & 0b11111111;
        uint8_t byte4 = ((uint8_t)this->header.bf.transport_scrambling_control << 6)
        | ((uint8_t)this->header.bf.adaptation_field_control << 4)
        | ((uint8_t)this->header.bf.continuity_counter & 0b1111);
        
        std::cout << "\033[34m";
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte1) << ' ' 
                              << std::setw(2) << std::setfill('0') << static_cast<int>(byte2) << ' ' 
                              << std::setw(2) << std::setfill('0') << static_cast<int>(byte3) << ' ' 
                              << std::setw(2) << std::setfill('0') << static_cast<int>(byte4) << ' ';
        std::cout << "\033[0m";              
        
        size_t counter = 4;
        
        std::cout << "\033[32m";
        if (adaptation_field && adaptation_field_length > 0) {
            for (size_t i = 0; i < adaptation_field_length; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(adaptation_field[i]) << " ";
                if ((counter + 1) % 16 == 0) std::cout << "\n";
                counter++;
            }
        }
        std::cout << "\033[0m";
    
        if (payload && payload_length > 0) {
            for (size_t i = 0; i < payload_length; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(payload[i]) << " ";
                if ((counter + 1) % 16 == 0) std::cout << "\n";
                counter++;
            }
            std::cout << std::dec << std::endl;
        }
    }
};

#endif // _TSPACKET_H