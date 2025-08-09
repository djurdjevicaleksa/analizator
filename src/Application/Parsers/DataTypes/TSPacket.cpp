#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <stdexcept>

#include "TSPacket.h"

TSPacket::TSPacketHeader::TSPacketHeader(const std::uint8_t* start)
    : sync_byte(start[0]),
    transport_error_indicator((start[1] >> 7) & 0b1),
    payload_unit_start_indicator((start[1] >> 6) & 0b1),
    transport_priority((start[1] >> 5) & 0b1),
    pid((static_cast<std::uint16_t>(start[1] & 0b11111) << 8) | start[2]),
    transport_scrambling_control((start[3] >> 6) & 0b11),
    adaptation_field_control((start[3] >> 4) & 0b11),
    continuity_counter(start[3] & 0b1111) 
    {}


TSPacket::AdaptationField::AdaptationExtension::AdaptationExtension(const std::uint8_t length, const std::uint8_t* start)
    : adaptation_extension_length(length) {

    if (length == 0) {
        this->legal_time_window_flag = std::nullopt;
        this->piecewise_rate_flag = std::nullopt;
        this->seamless_splice_flag = std::nullopt;

        this->legal_time_window = std::nullopt;
        this->piecewise_rate = std::nullopt;
        this->seamless_splice = std::nullopt;

        return;
    }

    this->legal_time_window_flag = std::make_optional<bool>((start[0] >> 7) & 0b1);
    this->piecewise_rate_flag = std::make_optional<bool>((start[0] >> 6) & 0b1);
    this->seamless_splice_flag = std::make_optional<bool>((start[0] >> 5) & 0b1);

    
    std::uint8_t* sliding_pointer = const_cast<std::uint8_t*>(&start[1]);

    if (this->legal_time_window_flag.value()) {
        this->legal_time_window = std::make_optional<AdaptationExtension::LegalTimeWindow>(
            sliding_pointer
        );

        sliding_pointer += 2;
    } else this->legal_time_window = std::nullopt;

    if (this->piecewise_rate_flag.value()) {
        this->piecewise_rate = std::make_optional<std::uint32_t>(
            static_cast<std::uint32_t>((sliding_pointer[0] & 0b111111) << 16)
            | static_cast<std::uint32_t>(sliding_pointer[1] << 8)
            | sliding_pointer[2]
        );

        sliding_pointer += 3;
    } else this->piecewise_rate = std::nullopt;

    if (this->seamless_splice_flag.value()) {
        this->seamless_splice = std::make_optional<AdaptationExtension::SeamlessSplice>(
            sliding_pointer
        );

        sliding_pointer += 5;
    } else this->seamless_splice = std::nullopt;
}


TSPacket::AdaptationField::AdaptationField(const std::uint8_t length, const std::uint8_t* start)
    : adaptation_field_length(length) {

    this->bit_representation = std::make_unique<std::uint8_t[]>(length + 1);
    std::memcpy(this->bit_representation.get(), &start[-1], length + 1);

    if (length == 0) {
        this->discontinuity_indicator = std::nullopt;
        this->random_access_indicator = std::nullopt;
        this->elementary_stream_priority_indicator = std::nullopt;
        this->pcr_flag = std::nullopt;
        this->opcr_flag = std::nullopt;
        this->splicing_point_flag = std::nullopt;
        this->transport_private_data_flag = std::nullopt;
        this->adaptation_field_extension_flag = std::nullopt;

        this->pcr = std::nullopt;
        this->opcr = std::nullopt;
        this->splice_countdown = std::nullopt;
        this->transport_private_data = std::nullopt;
        this->adaptation_extension = std::nullopt;
        
        return;
    }

    this->discontinuity_indicator = std::make_optional<bool>((start[0] >> 7) & 0b1);
    this->random_access_indicator = std::make_optional<bool>((start[0] >> 6) & 0b1);
    this->elementary_stream_priority_indicator = std::make_optional<bool>((start[0] >> 5) & 0b1);
    this->pcr_flag = std::make_optional<bool>((start[0] >> 4) & 0b1);
    this->opcr_flag = std::make_optional<bool>((start[0] >> 3) & 0b1);
    this->splicing_point_flag = std::make_optional<bool>((start[0] >> 2) & 0b1);
    this->transport_private_data_flag = std::make_optional<bool>((start[0] >> 1) & 0b1);
    this->adaptation_field_extension_flag = std::make_optional<bool>(start[0] & 0b1);


    std::uint8_t* sliding_pointer = const_cast<std::uint8_t*>(&start[1]);

    if (this->pcr_flag.value()) {
        this->pcr = std::make_optional<std::uint64_t>(
              (static_cast<std::uint64_t>(sliding_pointer[0]) << 40)
            | (static_cast<std::uint64_t>(sliding_pointer[1]) << 32)
            | (static_cast<std::uint64_t>(sliding_pointer[2]) << 24)
            | (static_cast<std::uint64_t>(sliding_pointer[3]) << 16)
            | (static_cast<std::uint64_t>(sliding_pointer[4]) << 8)
            | sliding_pointer[5]
        );
        
        sliding_pointer += 6;
    } else this->pcr = std::nullopt;

    if (this->opcr_flag.value()) {
        this->opcr = std::make_optional<std::uint64_t>(
              (static_cast<std::uint64_t>(sliding_pointer[0]) << 40)
            | (static_cast<std::uint64_t>(sliding_pointer[1]) << 32)
            | (static_cast<std::uint64_t>(sliding_pointer[2]) << 24)
            | (static_cast<std::uint64_t>(sliding_pointer[3]) << 16)
            | (static_cast<std::uint64_t>(sliding_pointer[4]) << 8)
            | sliding_pointer[5]
        );
        
        sliding_pointer += 6;
    } else this->opcr = std::nullopt;

    if (this->splicing_point_flag.value()) {
        this->splice_countdown = std::make_optional<std::int8_t>(
            static_cast<std::int8_t>(sliding_pointer[0])
        );

        sliding_pointer += 1;
    } else this->splice_countdown = std::nullopt;

    if (this->transport_private_data_flag.value()) {
        this->transport_private_data = std::make_optional<AdaptationField::TransportPrivateData>(
            sliding_pointer[0],
            sliding_pointer + 1
        );

        sliding_pointer += sliding_pointer[0];
    } else this->transport_private_data = std::nullopt;

    if (this->adaptation_field_extension_flag.value()) {
        this->adaptation_extension = std::make_optional<AdaptationExtension>(
            sliding_pointer[0],
            sliding_pointer + 1
        );

        sliding_pointer += sliding_pointer[0];
    } else this->adaptation_extension = std::nullopt;
}

TSPacket::TSPacket(const std::uint8_t* start, const std::uint8_t packet_size) : header(start) {

    if (this->header.transport_error_indicator) throw std::runtime_error("Skipped a packet due to an existing transport error.");
    if (this->header.transport_scrambling_control) throw std::runtime_error("Skipped a packet due to it being scrambled.");

    std::uint8_t* sliding_pointer = const_cast<std::uint8_t*>(start) + TSPacketHeader::size;
    
    // Higher bit indicates the presence of an adaptation field
    if (this->header.adaptation_field_control & 0b10) {
        this->adaptation_field = std::make_optional<AdaptationField>(
            sliding_pointer[0],
            sliding_pointer + 1
        );

        sliding_pointer += sliding_pointer[0] + 1;
    } else this->adaptation_field = std::nullopt;

    // Lower bit indicates the presence of a payload field
    if (this->header.adaptation_field_control & 0b1) {

        assert(sliding_pointer < start + packet_size);

        this->payload = std::make_optional<std::vector<std::uint8_t>>(
            sliding_pointer,
            const_cast<std::uint8_t*>(start) +  packet_size
        );
    }
}

std::string bool_string(bool in) {
    return in? "yes" : "no";
}

// Print BITS bits from the lowest "leftwards"
template<typename T>
std::string to_binary_low(T input, std::uint8_t BITS) {
    std::string out;
    for (std::size_t i = 0; i < BITS; i++) {
        out += (input >> (BITS - (i + 1)) & 0b1)? "1" : "0";
    }
    return out;
}

void TSPacket::print(std::size_t indent_level) const {
        
    std::cout << "\033[34m";        
    std::cout << "========== HEADER ========== " << std::endl;
    std::cout << " Sync byte                    : " << std::hex << static_cast<int>(header.sync_byte) << std::endl;
    std::cout << " Transport error indicator    : " << bool_string(header.transport_error_indicator) << std::endl;
    std::cout << " Payload unit start indicator : " << bool_string(header.payload_unit_start_indicator) << std::endl;
    std::cout << " Transport priority           : " << bool_string(header.transport_priority) << std::endl;
    std::cout << " PID=0x" << std::hex << std::setw(4) << std::setfill('0') << header.pid << std::endl;
    std::cout << " Transport scrambling control : " << to_binary_low<std::uint8_t>(header.transport_scrambling_control, 2) << std::endl;
    std::cout << " Adaptation field control     : " << to_binary_low<std::uint8_t>(header.adaptation_field_control, 2) << std::endl;
    std::cout << " Continuity counter           : " << std::dec << static_cast<int>(header.continuity_counter) << std::endl;
    std::cout << "==========/HEADER/========== " << std::endl;
    std::cout << "\033[0m" << std::endl;

    std::cout << "========== RAW PACKET ========== " << std::endl;
    uint8_t byte1 = (uint8_t)this->header.sync_byte;
    uint8_t byte2 = ((uint8_t)this->header.transport_error_indicator << 7)
    | ((uint8_t)this->header.payload_unit_start_indicator << 6)
    | ((uint8_t)this->header.transport_priority << 5)
    | ((this->header.pid >> 8) & 0b11111);
    uint8_t byte3 = this->header.pid & 0b11111111;
    uint8_t byte4 = ((uint8_t)this->header.transport_scrambling_control << 6)
    | ((uint8_t)this->header.adaptation_field_control << 4)
    | ((uint8_t)this->header.continuity_counter & 0b1111);
    
    std::cout << "\033[34m";
    std::cout << std::hex  << std::uppercase
                           << std::setw(2) << std::setfill('0') << static_cast<int>(byte1) 
                           << std::setw(2) << std::setfill('0') << static_cast<int>(byte2) << ' ' 
                           << std::setw(2) << std::setfill('0') << static_cast<int>(byte3) 
                           << std::setw(2) << std::setfill('0') << static_cast<int>(byte4) << ' ';
    std::cout << "\033[0m";              
    
    
    std::size_t counter = 4;
    std::size_t two_byte_spacing = 0;

    std::cout << "\033[32m";
    if (this->adaptation_field.has_value()) {
        for (std::size_t i = 0; i < this->adaptation_field.value().adaptation_field_length + 1; i++) {
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(adaptation_field.value().bit_representation[i]);

            if (++two_byte_spacing % 2 == 0) std::cout << ' ';
            if (!(++counter % 16)) std::cout << '\n';
        }
    }
    std::cout << "\033[0m";


    if (this->payload.has_value()) {
        for (std::size_t i = 0; i < this->payload.value().size(); i++) {
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(payload.value()[i]);

            if (++two_byte_spacing % 2 == 0) std::cout << ' ';
            if (!(++counter % 16)) std::cout << '\n';
        }
    }
    std::cout << std::dec << std::endl;
}