#ifndef _PSI_TABLE_HEADER_H
#define _PSI_TABLE_HEADER_H

#include <cstdint>
#include <optional>


struct PSITableHeader {
    std::uint8_t table_id;
    bool section_syntax_indicator;
    std::uint16_t section_length;

    PSITableHeader(const std::uint8_t* start);
    
    PSITableHeader() = delete;
    PSITableHeader(PSITableHeader&) = delete;
    PSITableHeader& operator=(PSITableHeader&) = delete;

    PSITableHeader(PSITableHeader&&) = default;
    PSITableHeader& operator=(PSITableHeader&&) = default;
    ~PSITableHeader() = default;

    void print(std::size_t indent_level) const;
};

struct PSITableLongHeader {
    std::uint16_t table_id_extension;
    std::uint8_t version_number;
    bool current_next_indicator;
    std::uint8_t section_number;
    std::uint8_t last_section_number;

    PSITableLongHeader(const std::uint8_t* start);

    PSITableLongHeader() = delete;
    PSITableLongHeader(PSITableLongHeader&) = delete;
    PSITableLongHeader& operator=(PSITableLongHeader&) = delete;

    PSITableLongHeader(PSITableLongHeader&&) = default;
    PSITableLongHeader& operator=(PSITableLongHeader&&) = default;
    ~PSITableLongHeader() = default;

    void print(std::size_t indent_level) const;
};

#endif // _PSI_TABLE_HEADER_H