#ifndef _SDT_H
#define _SDT_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <optional>
#include <memory>

#include "src/Application/Parsers/DataTypes/PSITableHeader.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

struct ServiceDescriptionSection {
    struct Service {
        std::uint16_t service_id;
        bool eit_schedule_flag;
        bool eit_present_following_flag;
        std::uint8_t running_status;
        bool free_ca_mode;
        std::uint16_t descriptors_loop_length;
        std::vector<std::unique_ptr<Descriptors::Descriptor>> descriptors;

        Service(std::uint8_t* start);

        Service() = delete;
        Service(const Service&) = delete;
        Service& operator=(const Service&) = delete;

        Service(Service&&) = default;
        Service& operator=(Service&&) = default;
        ~Service() = default;

        void print(const std::size_t indent_level) const;
    };
    
    PSITableHeader header;
    std::optional<PSITableLongHeader> long_header;
    std::uint16_t original_network_id;
    std::uint16_t services_length;
    std::vector<Service> services;
    uint32_t crc;

    ServiceDescriptionSection(const std::uint8_t* start);

    ServiceDescriptionSection() = delete;
    ServiceDescriptionSection(const ServiceDescriptionSection&) = delete;
    ServiceDescriptionSection& operator=(const ServiceDescriptionSection&) = delete;

    ServiceDescriptionSection(ServiceDescriptionSection&&) = default;
    ServiceDescriptionSection& operator=(ServiceDescriptionSection&&) = default;
    ~ServiceDescriptionSection() = default;

    void print(const std::size_t indent_level) const;
};

#endif // _SDT_H

