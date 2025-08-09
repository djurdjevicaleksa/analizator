#ifndef _NIT_H
#define _NIT_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>

#include "src/Application/Parsers/DataTypes/PSITableHeader.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

struct NetworkInformationSection {
    struct TSLoopEntry {
        uint16_t tsid;
        uint16_t onid;
        uint16_t descriptors_length;
        std::vector<std::unique_ptr<Descriptors::Descriptor>> descriptors;

        TSLoopEntry(const std::uint8_t* start);

        TSLoopEntry() = delete;
        TSLoopEntry(TSLoopEntry&) = delete;
        TSLoopEntry& operator=(TSLoopEntry&) = delete;

        ~TSLoopEntry() = default;

        void print(std::size_t indent_level) const;
    };
        
    PSITableHeader header;
    std::optional<PSITableLongHeader> long_header;
    std::uint16_t network_descriptors_length;
    std::vector<std::unique_ptr<Descriptors::Descriptor>> network_descriptors;
    std::uint16_t transport_stream_loop_length;
    std::vector<std::unique_ptr<TSLoopEntry>> ts_loop_entries;
    uint32_t crc;

    NetworkInformationSection(const std::uint8_t* start);

    NetworkInformationSection() = delete;
    NetworkInformationSection(NetworkInformationSection&) = delete;
    NetworkInformationSection& operator=(NetworkInformationSection&) = delete;

    NetworkInformationSection(NetworkInformationSection&&) = default;
    NetworkInformationSection& operator=(NetworkInformationSection&&) = default;
    ~NetworkInformationSection() = default;

    void print(std::size_t indent_level) const;
};

#endif // _NIT_H