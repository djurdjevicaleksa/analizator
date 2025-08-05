#include <cstdint>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <iostream>

#include "ExtendedEventLinkageInfo.h"
#include "src/Application/Utilities/Utils.h"

SubStructs::ExtendedEventLinkageInfo::ExtendedEventLinkageInfoEntry::ExtendedEventLinkageInfoEntry(const std::uint8_t* start) 
    : SubStructs::EventLinkageInfo(start) {

    this->link_type = (start[2] >> 4) & 0b11;
    this->target_id_type = (start[2] >> 2) & 0b11;
    this->original_network_id_flag = (start[2] >> 1) & 0b1;
    this->service_id_flag = start[2] & 0b1;

    this->end_pointer = const_cast<std::uint8_t*>(&start[3]);

    if (this->target_id_type == 3) {
        this->user_defined_id = std::optional<std::uint16_t>((start[3] << 8) | start[4]);
        this->target_transport_stream_id = std::nullopt;
        this->target_original_network_id = std::nullopt;
        this->target_service_id = std::nullopt;

        this->end_pointer += 2;
    }
    else {
        std::uint8_t* it = const_cast<std::uint8_t*>(&start[3]);

        if (this->target_id_type == 1) {
            this->target_transport_stream_id = std::optional<std::uint16_t>((it[0] << 8) | it[1]);
            it += 2;
            this->end_pointer += 2;
        }
        else this->target_transport_stream_id = std::nullopt;

        if (this->original_network_id_flag) {
            this->target_original_network_id = std::optional<std::uint16_t>((it[0] << 8) | it[1]);
            it += 2;
            this->end_pointer += 2;
        }
        else this->target_original_network_id = std::nullopt;

        if (this->service_id_flag) {
            this->target_service_id = std::optional<std::uint16_t>((it[0] << 8) | it[1]);
            this->end_pointer += 2;
        }
        else this->target_service_id = std::nullopt;
    }
}

std::string SubStructs::ExtendedEventLinkageInfo::ExtendedEventLinkageInfoEntry::deduceLinkType() const {
    switch(this->link_type) {
        case 0: return "SD";
        case 1: return "HD";
        case 2: return "3D";
        default: return "Reserved for future use";
    }    
}

std::string SubStructs::ExtendedEventLinkageInfo::ExtendedEventLinkageInfoEntry::deduceTargetIdType() const {
    switch(this->target_id_type) {
        case 0: return "Use transport_stream_id";
        case 1: return "Use target_transport_stream_id";
        case 2: return "Wildcard-match any transport_stream_id";
        case 3: return "Use user_defined_id";
        default: throw std::runtime_error("[ExtendedEventLinkageInfo] Failed to deduce the target id type: Invalid target id type value.");
    }
}

void SubStructs::ExtendedEventLinkageInfo::ExtendedEventLinkageInfoEntry::print(std::size_t indent_level) const {

    utils::printLine("Extended event linkage info entry", indent_level, '=');
    utils::printDataPoint("Link type", this->link_type, indent_level, this->deduceLinkType());
    utils::printDataPoint("Target ID type", this->target_id_type, indent_level, this->deduceTargetIdType());
    utils::printDataPoint("Original network ID flag", this->original_network_id_flag, indent_level);
    utils::printDataPoint("Service ID flag", this->service_id_flag, indent_level);
    if (this->user_defined_id.has_value()) utils::printDataPoint("User-defined ID", this->user_defined_id.value());
    if (this->target_transport_stream_id.has_value()) utils::printDataPoint("Target transport stream ID", this->target_transport_stream_id.value());
    if (this->target_original_network_id.has_value()) utils::printDataPoint("Target original network ID", this->target_original_network_id.value());
    if (this->target_service_id.has_value()) utils::printDataPoint("Target service ID", this->target_service_id.value());
    utils::printLine("Extended event linkage info entry", indent_level, '=');
}

// ============================

SubStructs::ExtendedEventLinkageInfo::ExtendedEventLinkageInfo(const std::uint8_t* start) {

    this->loop_length = start[0];

    std::uint8_t* loop_start = const_cast<std::uint8_t*>(start + 1);
    const std::uint8_t* loop_end = start + this->loop_length;

    while (loop_start < loop_end) {
        this->loop.emplace_back(loop_start);
        auto& last_entry = *this->loop.rbegin();
        
        loop_start = last_entry.end_pointer;
    }
}

void SubStructs::ExtendedEventLinkageInfo::print(std::size_t indent_level) const {
    utils::printLine("Extended event linkage info", indent_level, '=');
    utils::printDataPoint("Loop length", this->loop_length, indent_level);
    for (std::size_t i = 0; i < this->loop.size(); i++) this->loop[i].print(indent_level + 1);
    utils::printLine("Extended event linkage info", indent_level, '=');
}