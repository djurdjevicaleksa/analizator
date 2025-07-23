#include "NIT.h"

#include <iostream>
#include <iomanip>
#include <cstring>

namespace {
    template<typename T>
    void printHex(const std::string& label, T value, int indent = 1) {
        std::cout << std::string(indent * 4, ' ')
            << label << ": 0x"
            << std::hex << std::uppercase << std::setfill('0') 
            << std::setw(sizeof(T) * 2)
            << static_cast<uint64_t>(value)
            << std::dec << std::endl;
    }

    void printLine(const std::string& label, int indent = 1, char border = '=') {
        std::cout << std::string(indent * 4, ' ')
            << std::string(5, border) << " " << label << " "
            << std::string(5, border) << std::endl;
    }
}

void NetworkInformationSection::PacketHeader::print() const {
    printLine("NIT TABLE HEADER");
    printHex("table_id", this->table_id);
    printHex("section_syntax_indicator", this->section_syntax_indicator);
    printHex("section_length", this->section_length);
    printHex("network_id", this->network_id);
    printHex("version_number", this->version_number);
    printHex("current_next_indicator", this->current_next_indicator);
    printHex("section_number", this->section_number);
    printHex("last_section_number", this->last_section_number);
    printLine("/NIT TABLE HEADER/", 1, '=');
}

void NetworkInformationSection::Descriptor::print() const {
    printLine("DESCRIPTOR", 2, '-');
    printHex("descriptor_tag", this->tag, 2);
    printHex("descriptor_length", this->length, 2);
    std::cout << std::string(2 * 4, ' ') << "descriptor_data: ";
    for (auto ch : this->data) std::cout << ch;
    std::cout << std::endl;
    printLine("/DESCRIPTOR/", 2, '-');
}

void NetworkInformationSection::TSLoopEntry::print() const {
    printLine("TS LOOP ENTRY");
    printHex("tsid", this->tsid);
    printHex("onid", this->onid);
    printHex("descriptors_length", this->length);
    for (const auto& desc : this->descriptors) {
        desc.print();
    }
    printLine("/TS LOOP ENTRY/");
}

void NetworkInformationSection::print() const {
    printLine("NIT TABLE", 0, '=');

    this->header.print();

    printLine("NETWORK DESCRIPTORS");
    printHex("network_descriptors_length", this->header.network_descriptors_length);
    for (const auto& desc : this->network_descriptors) {
        desc.print();
    }
    printLine("/NETWORK DESCRIPTORS/");

    printLine("TS DESCRIPTORS");
    printHex("ts_loop_length", this->header.transport_stream_loop_length);
    for (const auto& entry : this->ts_loop_entries) {
        entry.print();
    }
    printLine("/TS DESCRIPTORS/");

    printHex("CRC", this->crc);
    printLine("/NIT TABLE/", 0, '=');
}
