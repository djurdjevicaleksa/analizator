#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>

#include "src/Application/Parsers/DataTypes/NIT.h"
#include "src/Application/Utilities/Utils.h"
#include "src/Application/Parsers/DataTypes/Descriptors/DescriptorFactory.h"

NetworkInformationSection::NetworkInformationSection(const std::uint8_t* start) : header(start) {

    std::uint8_t* moving_ptr = const_cast<std::uint8_t*>(start + 3);

    if (this->header.section_syntax_indicator) {
        this->long_header.emplace(moving_ptr);
        moving_ptr += 5;
    } else this->long_header = std::nullopt;

    this->network_descriptors_length = ((moving_ptr[0] & 0b1111) << 8) | moving_ptr[1];
    
    moving_ptr += 2;
    const std::uint8_t* network_descriptors_end = moving_ptr + this->network_descriptors_length;
    
    while(moving_ptr + 1 < network_descriptors_end) {
        if (moving_ptr + 2 + moving_ptr[1] > network_descriptors_end) {
            throw std::runtime_error("[NIT] Error while parsing a NIT table: Malformed network descriptor.");
        }

        std::uint8_t tag = moving_ptr[0];
        std::uint8_t length = moving_ptr[1];

        this->network_descriptors.emplace_back(Descriptors::DescriptorFactory::instance().make(
            tag, length, &moving_ptr[2]
        ));

        moving_ptr += 2 + length;
    }

    moving_ptr = const_cast<std::uint8_t*>(network_descriptors_end);

    this->transport_stream_loop_length = ((moving_ptr[0] & 0b1111) << 8) | moving_ptr[1];

    moving_ptr += 2;

    const std::uint8_t* ts_loop_end = moving_ptr + this->transport_stream_loop_length;

    while (moving_ptr + 5 < ts_loop_end) {
        this->ts_loop_entries.emplace_back(std::make_unique<TSLoopEntry>(moving_ptr));

        moving_ptr += 6 + (this->ts_loop_entries.rbegin())->get()->descriptors_length;
    }

    this->crc = static_cast<uint32_t>(moving_ptr[0] << 24)
        | static_cast<uint32_t>(moving_ptr[1] << 16)
        | static_cast<uint32_t>(moving_ptr[2] << 8)
        | static_cast<uint32_t>(moving_ptr[3]);
}


NetworkInformationSection::TSLoopEntry::TSLoopEntry(const std::uint8_t* start) {

    this->tsid = (start[0] << 8) | start[1];
    this->onid = (start[2] << 8) | start[3];
    this->descriptors_length = ((start[4] & 0b1111) << 8) | start[5];

    std::uint8_t* moving_ptr = const_cast<std::uint8_t*>(&start[6]);

    const std::uint8_t* descriptors_end = moving_ptr + this->descriptors_length;

    while (moving_ptr < descriptors_end) {
        if (moving_ptr + 2 + moving_ptr[1] > descriptors_end) {
            throw std::runtime_error("[NIT] Error while parsing a NIT table: Malformed TS loop entry descriptor.");
        }

        std::uint8_t tag = moving_ptr[0];
        std::uint8_t length = moving_ptr[1];

        this->descriptors.emplace_back(Descriptors::DescriptorFactory::instance().make(
            tag, length, &moving_ptr[2]
        ));

        moving_ptr += 2 + length;
    }
}



// std::string NetworkInformationSection::getDescriptorTypeFromTag(uint8_t tag) {
//     switch (tag)
//     {
//         case 0x40: return std::string("network_name_descriptor");
//         case 0x41: return std::string("service_list_descriptor");
//         case 0x42: return std::string("stuffing_descriptor");
//         case 0x43: return std::string("satellite_delivery_system_descriptor");
//         case 0x44: return std::string("cable_delivery_system_descriptor");
//         case 0x4A: return std::string("linkage_descriptor");
//         case 0x5A: return std::string("terrestrial_delivery_system_descriptor");
//         case 0x5B: return std::string("multilingual_network_name_descriptor");
//         case 0x5F: return std::string("private_data_specifier_descriptor");
//         case 0x62: return std::string("frequency_list_descriptor");
//         case 0x6C: return std::string("cell_list_descriptor");
//         case 0x6D: return std::string("cell_frequency_link_descriptor");
//         case 0x73: return std::string("default_authority_descriptor");
//         case 0x77: return std::string("time_slice_fec_identifier_descriptor");
//         case 0x79: return std::string("S2_satellite_delivery_system_descriptor");
//         case 0x7D: return std::string("XAIT_location_descriptor");
//         case 0x7E: return std::string("FTA_content_management_descriptor");
//         case 0x7F: return std::string("extension_descriptor");
//         default: return std::string("unknown descriptor");
//     }
// }

void NetworkInformationSection::TSLoopEntry::print(std::size_t indent_level) const {
    utils::printLine("TS loop entry", indent_level, '-');
    utils::printDataPoint("tsid", this->tsid, indent_level);
    utils::printDataPoint("onid", this->onid, indent_level);
    utils::printDataPoint("Descriptors length", this->descriptors_length, indent_level);
    for (std::size_t i = 0; i < this->descriptors.size(); i++) this->descriptors[i].get()->print(indent_level + 1);
    utils::printLine("TS loop entry", indent_level, '-');
}

void NetworkInformationSection::print(std::size_t indent_level) const {
    utils::printLine("Network information table", indent_level, '=');
    this->header.print(indent_level + 1);
    if (this->long_header.has_value()) this->long_header.value().print(indent_level + 1);
    utils::printDataPoint("Network descriptors length", this->network_descriptors_length, indent_level);
    for (std::size_t i = 0; i < this->network_descriptors.size(); i++) this->network_descriptors[i].get()->print(indent_level + 1);
    utils::printDataPoint("Transport stream loop length", this->transport_stream_loop_length, indent_level);
    for (std::size_t i = 0; i < this->ts_loop_entries.size(); i++) this->ts_loop_entries[i].get()->print(indent_level + 1);
    utils::printDataPoint("CRC", this->crc, indent_level);
    utils::printLine("Network information table", indent_level, '=');

}