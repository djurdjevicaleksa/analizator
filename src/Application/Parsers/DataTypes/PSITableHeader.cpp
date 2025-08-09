#include <cstdint>
#include <cstddef>
#include <string>

#include "PSITableHeader.h"
#include "src/Application/Utilities/Utils.h"

PSITableHeader::PSITableHeader(const std::uint8_t* start) {
    this->table_id = start[0];
    this->section_syntax_indicator = (start[1] >> 7) & 0b1;
    this->section_length = ((start[1] & 0b1111) << 8) | start[2];
}

PSITableLongHeader::PSITableLongHeader(const std::uint8_t* start) {
    this->table_id_extension = (start[0] << 8) | start[1];
    this->version_number = (start[2] >> 1) & 0b11111;
    this->current_next_indicator = start[2] & 0b1;
    this->section_number = start[3];
    this->last_section_number = start[4];
}

void PSITableHeader::print(std::size_t indent_level) const {
    utils::printLine("Header", indent_level, '-');
    utils::printDataPoint("Table ID", this->table_id, indent_level);
    utils::printDataPoint("Section syntax indicator", this->section_syntax_indicator, indent_level);
    utils::printDataPoint("Section length", this->section_length, indent_level);
    utils::printLine("Header", indent_level, '-');
}

void PSITableLongHeader::print(std::size_t indent_level, const std::string& table_id_extension_name) const {
    utils::printLine("Extended header", indent_level, '-');
    utils::printDataPoint(table_id_extension_name, this->table_id_extension, indent_level);
    utils::printDataPoint("Version number", this->version_number, indent_level);
    utils::printDataPoint("Current next indicator", this->current_next_indicator, indent_level);
    utils::printDataPoint("Section number", this->section_number, indent_level);
    utils::printDataPoint("Last section number", this->last_section_number, indent_level);
    utils::printLine("Extended header", indent_level, '-');
}

