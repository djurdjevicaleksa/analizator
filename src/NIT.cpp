#include <iostream>
#include <iomanip>
#include <cstring>

#include "NIT.h"
#include "Utils.h"

void NetworkInformationSection::PacketHeader::print() const {
    utils::printLine("NIT TABLE HEADER");
    utils::printHex("table_id", this->table_id);
    utils::printHex("section_syntax_indicator", this->section_syntax_indicator);
    utils::printHex("section_length", this->section_length);
    utils::printHex("network_id", this->network_id);
    utils::printHex("version_number", this->version_number);
    utils::printHex("current_next_indicator", this->current_next_indicator);
    utils::printHex("section_number", this->section_number);
    utils::printHex("last_section_number", this->last_section_number);
    utils::printLine("/NIT TABLE HEADER/", 1, '=');
}

void NetworkInformationSection::Descriptor::print() const {
    utils::printLine("DESCRIPTOR", 2, '-');
    utils::printHex("descriptor_tag", this->tag, 2);
    utils::printHex("descriptor_length", this->length, 2);
    std::cout << std::string(2 * 4, ' ') << "descriptor_data: ";
    for (auto ch : this->data) std::cout << ch;
    std::cout << std::endl;
    utils::printLine("/DESCRIPTOR/", 2, '-');
}

void NetworkInformationSection::TSLoopEntry::print() const {
    utils::printLine("TS LOOP ENTRY");
    utils::printHex("tsid", this->tsid);
    utils::printHex("onid", this->onid);
    utils::printHex("descriptors_length", this->length);
    for (const auto& desc : this->descriptors) {
        desc.print();
    }
    utils::printLine("/TS LOOP ENTRY/");
}

void NetworkInformationSection::print() const {
    utils::printLine("NIT TABLE", 0, '=');

    this->header.print();

    utils::printLine("NETWORK DESCRIPTORS");
    utils::printHex("network_descriptors_length", this->header.network_descriptors_length);
    for (const auto& desc : this->network_descriptors) {
        desc.print();
    }
    utils::printLine("/NETWORK DESCRIPTORS/");

    utils::printLine("TS DESCRIPTORS");
    utils::printHex("ts_loop_length", this->header.transport_stream_loop_length);
    for (const auto& entry : this->ts_loop_entries) {
        entry.print();
    }
    utils::printLine("/TS DESCRIPTORS/");

    utils::printHex("CRC", this->crc);
    utils::printLine("/NIT TABLE/", 0, '=');
}
