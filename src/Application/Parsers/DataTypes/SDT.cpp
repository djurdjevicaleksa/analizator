#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "src/Application/Parsers/DataTypes/SDT.h"
#include "src/Application/Utilities/Utils.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Factory.h"

ServiceDescriptionSection::ServiceDescriptionSection(const std::uint8_t* start) : header(start) {

    std::uint8_t* moving_ptr = const_cast<std::uint8_t*>(start + 3);

    if (this->header.section_syntax_indicator) {
        this->long_header.emplace(moving_ptr);
        moving_ptr += 5;
    } else this->long_header = std::nullopt;

    this->original_network_id = (moving_ptr[0] << 8) | moving_ptr[1];
    moving_ptr += 3;

    // section_length lower bit + section length + 1 as the outer end pointer
    const std::uint8_t* services_end = &start[3] + this->header.section_length - 4;

    this->services_length = services_end - moving_ptr;

    while(moving_ptr < services_end) {
        this->services.emplace_back(moving_ptr);

        moving_ptr += 5 + (this->services.rbegin()->descriptors_loop_length);
    }

    this->crc = (static_cast<std::uint32_t>(moving_ptr[0]) << 24)
              | (static_cast<std::uint32_t>(moving_ptr[1]) << 16)
              | (static_cast<std::uint32_t>(moving_ptr[2]) << 8)
              | static_cast<std::uint32_t>(moving_ptr[3]);
}

ServiceDescriptionSection::Service::Service(std::uint8_t* start) {

    this->service_id = (start[0] << 8) | start[1];
    this->eit_schedule_flag = (start[2] >> 1) & 0b1;
    this->eit_present_following_flag = start[2] & 0b1;
    this->running_status = (start[3] >> 5) & 0b111;
    this->free_ca_mode = (start[3] >> 4) & 0b1;
    this->descriptors_loop_length = ((start[3] & 0b1111) << 8) | start[4];

    std::uint8_t* moving_ptr = const_cast<std::uint8_t*>(&start[5]);
    const std::uint8_t* descriptors_end = moving_ptr + descriptors_loop_length;
    assert(moving_ptr < descriptors_end);

    while (moving_ptr < descriptors_end) {
        if (moving_ptr + 2 + moving_ptr[1] > descriptors_end) {
            throw std::runtime_error("Failed to construct an SDT service: Malformed descriptor data.");
        }

        std::uint8_t tag = moving_ptr[0];
        std::uint8_t length = moving_ptr[1];

        this->descriptors.emplace_back(Factory::create(tag, length, &moving_ptr[2]));
        moving_ptr += 2 + length;
    }
}

void ServiceDescriptionSection::Service::print(const std::size_t indent_level) const {
    utils::printLine("Service", indent_level, '=');
    utils::printDataPoint("Service ID", this->service_id, indent_level);
    utils::printDataPoint("EIT schedule flag", this->eit_schedule_flag, indent_level);
    utils::printDataPoint("EIT present following flag", this->eit_present_following_flag, indent_level);
    utils::printDataPoint("Running status", this->running_status, indent_level);
    utils::printDataPoint("Free CA mode", this->free_ca_mode, indent_level);
    utils::printDataPoint("Descriptors length", this->descriptors_loop_length, indent_level);
    for (std::size_t i = 0; i < this->descriptors.size(); i++) this->descriptors[i].get()->print(indent_level + 1); 
    utils::printLine("Service", indent_level, '=');
}

void ServiceDescriptionSection::print(const std::size_t indent_level) const {
    utils::printLine("Service Description Table", indent_level, '=');
    this->header.print(indent_level + 1);
    if (this->long_header.has_value()) this->long_header.value().print(indent_level + 1, "Transport stream ID");
    utils::printDataPoint("Original network ID", this->original_network_id, indent_level);
    utils::printDataPoint("Services length", this->services_length, indent_level);
    for (std::size_t i = 0; i < this->services.size(); i++) this->services[i].print(indent_level);
    utils::printDataPoint("CRC", this->crc, indent_level);
    utils::printLine("Service Description Table", indent_level, '=');
}