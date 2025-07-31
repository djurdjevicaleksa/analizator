#include <iostream>
#include <iomanip>

#include "SDTParser.h"
#include "Utils.h"

std::string ServiceDescriptionSection::getDescriptorTypeFromTag(uint8_t tag) {
    switch (tag)
    {
        case 0x48: return std::string("service_descriptor");
        case 0x49: return std::string("country_availability_descriptor");
        case 0x4A: return std::string("linkage_descriptor");
        case 0x4B: return std::string("NVOD_reference_descriptor");
        case 0x4C: return std::string("time_shifted_service_descriptor");
        case 0x50: return std::string("component_descriptor");
        case 0x51: return std::string("mosaic_descriptor");
        case 0x53: return std::string("CA_identifier_descriptor");
        case 0x57: return std::string("telephone_descriptor");
        case 0x5D: return std::string("multilingual_service_name_descriptor");
        case 0x5F: return std::string("private_data_specifier_descriptor");
        case 0x64: return std::string("data_broadcast_descriptor");
        case 0x6E: return std::string("announcement_support_descriptor");
        case 0x71: return std::string("service_identifier_descriptor");
        case 0x72: return std::string("service_availability_descriptor");
        case 0x73: return std::string("default_authority_descriptor");
        case 0x7D: return std::string("XAIT_location_descriptor"); 
        case 0x7E: return std::string("FTA_content_management_descriptor");
        case 0x7F: return std::string("extension_descriptor");
        default: return std::string("unknown descriptor");
    }
}

void ServiceDescriptionSection::PacketHeader::print() const {
    utils::printLine("SDT TABLE HEADER");
    utils::printDataPoint("table_id", table_id);
    utils::printDataPoint("section_syntax_indicator", section_syntax_indicator);
    utils::printDataPoint("section_length", section_length);
    utils::printDataPoint("transport_stream_id", transport_stream_id);
    utils::printDataPoint("version_number", version_number);
    utils::printDataPoint("current_next_indicator", current_next_indicator);
    utils::printDataPoint("section_number", section_number);
    utils::printDataPoint("last_section_number", last_section_number);
    utils::printDataPoint("original_network_id", original_network_id);
    utils::printLine("/SDT TABLE HEADER/", 1, '=');
}

void ServiceDescriptionSection::Service::print() const {
    utils::printLine("SERVICE", 2, '-');
    utils::printDataPoint("service_id", service_id, 2);
    utils::printDataPoint("eit_schedule_flag", eit_schedule_flag, 2);
    utils::printDataPoint("eit_present_following_flag", eit_present_following_flag, 2);
    utils::printDataPoint("running_status", running_status, 2);
    utils::printDataPoint("free_ca_mode", free_ca_mode, 2);
    utils::printDataPoint("descriptors_loop_length", descriptors_loop_length, 2);

    for (const auto& desc : descriptors) {
        desc.print(ServiceDescriptionSection::getDescriptorTypeFromTag);
    }

    utils::printLine("/SERVICE/", 2, '-');
}

void ServiceDescriptionSection::print() const {
    utils::printLine("SDT TABLE", 0, '=');

    header.print();

    utils::printLine("SERVICES", 1);
    for (const auto& service : services) {
        service.print();
    }
    utils::printLine("/SERVICES/", 1);

    utils::printDataPoint("CRC", crc);
    utils::printLine("/SDT TABLE/", 0, '=');
}