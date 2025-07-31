#include <iostream>
#include <iomanip>
#include <cstring>

#include "NIT.h"
#include "Utils.h"

std::string NetworkInformationSection::getDescriptorTypeFromTag(uint8_t tag) {
    switch (tag)
    {
        case 0x40: return std::string("network_name_descriptor");
        case 0x41: return std::string("service_list_descriptor");
        case 0x42: return std::string("stuffing_descriptor");
        case 0x43: return std::string("satellite_delivery_system_descriptor");
        case 0x44: return std::string("cable_delivery_system_descriptor");
        case 0x4A: return std::string("linkage_descriptor");
        case 0x5A: return std::string("terrestrial_delivery_system_descriptor");
        case 0x5B: return std::string("multilingual_network_name_descriptor");
        case 0x5F: return std::string("private_data_specifier_descriptor");
        case 0x62: return std::string("frequency_list_descriptor");
        case 0x6C: return std::string("cell_list_descriptor");
        case 0x6D: return std::string("cell_frequency_link_descriptor");
        case 0x73: return std::string("default_authority_descriptor");
        case 0x77: return std::string("time_slice_fec_identifier_descriptor");
        case 0x79: return std::string("S2_satellite_delivery_system_descriptor");
        case 0x7D: return std::string("XAIT_location_descriptor");
        case 0x7E: return std::string("FTA_content_management_descriptor");
        case 0x7F: return std::string("extension_descriptor");
        default: return std::string("unknown descriptor");
    }
}

void NetworkInformationSection::PacketHeader::print() const {
    utils::printLine("NIT TABLE HEADER");
    utils::printDataPoint("table_id", this->table_id);
    utils::printDataPoint("section_syntax_indicator", this->section_syntax_indicator);
    utils::printDataPoint("section_length", this->section_length);
    utils::printDataPoint("network_id", this->network_id);
    utils::printDataPoint("version_number", this->version_number);
    utils::printDataPoint("current_next_indicator", this->current_next_indicator);
    utils::printDataPoint("section_number", this->section_number);
    utils::printDataPoint("last_section_number", this->last_section_number);
    utils::printLine("/NIT TABLE HEADER/", 1, '=');
}

void NetworkInformationSection::TSLoopEntry::print() const {
    utils::printLine("TS LOOP ENTRY");
    utils::printDataPoint("tsid", this->tsid);
    utils::printDataPoint("onid", this->onid);
    utils::printDataPoint("descriptors_length", this->length);
    for (const auto& desc : this->descriptors) {
        desc.print(NetworkInformationSection::getDescriptorTypeFromTag);
    }
    utils::printLine("/TS LOOP ENTRY/");
}

void NetworkInformationSection::print() const {
    utils::printLine("NIT TABLE", 0, '=');

    this->header.print();

    utils::printLine("NETWORK DESCRIPTORS");
    utils::printDataPoint("network_descriptors_length", this->header.network_descriptors_length);
    for (const auto& desc : this->network_descriptors) {
        desc.print(NetworkInformationSection::getDescriptorTypeFromTag);
    }
    utils::printLine("/NETWORK DESCRIPTORS/");

    utils::printLine("TS DESCRIPTORS");
    utils::printDataPoint("ts_loop_length", this->header.transport_stream_loop_length);
    for (const auto& entry : this->ts_loop_entries) {
        entry.print();
    }
    utils::printLine("/TS DESCRIPTORS/");

    utils::printDataPoint("CRC", this->crc);
    utils::printLine("/NIT TABLE/", 0, '=');
}