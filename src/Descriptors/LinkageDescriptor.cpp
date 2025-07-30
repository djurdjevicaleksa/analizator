#include <cstdint>
#include <cstddef>
#include <optional>
#include <memory>

#include "LinkageDescriptor.h"

Descriptors::LinkageDescriptor::LinkageDescriptor(std::size_t length, const std::uint8_t* start) 
    : Descriptor(length) {

    this->transport_stream_id = (start[0] << 8) | start[1];
    this->original_network_id = (start[2] << 8) | start[3];
    this->service_id = (start[4] << 8) | start[5];
    this->linkage_type = start[6];

    const std::uint8_t* it = &start[7];
    std::uint8_t* private_data_start = const_cast<std::uint8_t*>(it);

    if (this->linkage_type == 0x08) {
        this->mobile_hand_over_info = std::move(std::optional<SubStructs::MobileHandOverLinkage>(it));
        this->event_linkage_info = std::nullopt;
        this->extended_event_linkage_info = std::nullopt;

        if (this->mobile_hand_over_info.has_value()) {
            private_data_start = this->mobile_hand_over_info.value().end_pointer;
        }
    }
    else if (this->linkage_type == 0x0D) {
        this->mobile_hand_over_info = std::nullopt;
        this->event_linkage_info = std::move(std::optional<SubStructs::EventLinkageInfo>(it));
        this->extended_event_linkage_info = std::nullopt;

        private_data_start += SubStructs::EventLinkageInfo::size;
    }
    else if (this->linkage_type == 0x0E) {
        this->mobile_hand_over_info = std::nullopt;
        this->event_linkage_info = std::nullopt;
        this->extended_event_linkage_info = std::move(std::optional<SubStructs::ExtendedEventLinkageInfo>(it));

        if (this->extended_event_linkage_info.has_value()) {
            private_data_start += this->extended_event_linkage_info.value().getLoopLength();
        }
    }

    const std::uint8_t* descriptor_end = start + length;
    const std::uint8_t private_data_size = descriptor_end - private_data_start;

    if (private_data_size) {
        this->private_data = std::make_unique<std::uint8_t[]>(descriptor_end - private_data_start);
        std::copy(static_cast<const std::uint8_t*>(private_data_start), descriptor_end, this->private_data.get());
    }
}

std::string Descriptors::LinkageDescriptor::deduceLinkageType() const {
    switch(this->linkage_type) {
        case 0x01: return "Information service";
        case 0x02: return "EPG service";
        case 0x03: return "CA replacement service";
        case 0x04: return "TS containing complete network/bouquet SI";
        case 0x05: return "Service replacement service";
        case 0x06: return "Data broadcast service";
        case 0x07: return "RCS map";
        case 0x08: return "Mobile hand-over";
        case 0x09: return "System software update service";
        case 0x0A: return "TS containing SSU, BAT or NIT";
        case 0x0B: return "IP/MAC notification service";
        case 0x0C: return "TS containing INT, BAT or NIT";
        case 0x0D: return "Event linkage*";
        case 0x0E: return "Extended event linkage*";
    }

    if (this->linkage_type >= 0x80 && this->linkage_type <= 0xFE) return "User defined";
    return "Reserved for future use";
}