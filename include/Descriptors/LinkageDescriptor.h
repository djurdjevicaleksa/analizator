#ifndef _LINKAGE_DESCRIPTOR_H
#define _LINKAGE_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>

#include "Descriptor.h"
#include "MobileHandOverLinkage.h"
#include "EventLinkageInfo.h"
#include "ExtendedEventLinkageInfo.h"

namespace Descriptors {

    class LinkageDescriptor: Descriptor {

        std::uint16_t transport_stream_id;
        std::uint16_t original_network_id;
        std::uint16_t service_id;
        std::uint8_t linkage_type;

        std::optional<SubStructs::MobileHandOverLinkage> mobile_hand_over_info;
        std::optional<SubStructs::EventLinkageInfo> event_linkage_info;
        std::optional<SubStructs::ExtendedEventLinkageInfo> extended_event_linkage_info;

        std::unique_ptr<std::uint8_t[]> private_data;

    public:
        static constexpr std::uint8_t tag = 0x4A;

        LinkageDescriptor(std::size_t length, const std::uint8_t* start);

        LinkageDescriptor(LinkageDescriptor&&) = default;
        LinkageDescriptor& operator=(LinkageDescriptor&&) = default;
        ~LinkageDescriptor() = default;

        std::string deduceLinkageType() const;
    };
}

#endif // _LINKAGE_DESCRIPTOR_H