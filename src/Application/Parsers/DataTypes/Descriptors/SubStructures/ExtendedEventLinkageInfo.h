#ifndef _EXTENDED_EVENT_LINKAGE_INFO_H
#define _EXTENDED_EVENT_LINKAGE_INFO_H

#include <cstdint>
#include <cstddef>
#include <optional>
#include <vector>
#include <string>

#include "EventLinkageInfo.h"

namespace SubStructs {

    struct ExtendedEventLinkageInfo {
        class ExtendedEventLinkageInfoEntry: EventLinkageInfo {

            std::uint8_t link_type;
            std::uint8_t target_id_type;
            bool original_network_id_flag;
            bool service_id_flag;
            std::optional<std::uint16_t> user_defined_id;
            std::optional<std::uint16_t> target_transport_stream_id;
            std::optional<std::uint16_t> target_original_network_id;
            std::optional<std::uint16_t> target_service_id;

        public:

            std::uint8_t* end_pointer;

        public:
        
            ExtendedEventLinkageInfoEntry(const std::uint8_t* start);

            ExtendedEventLinkageInfoEntry() = delete;
            ExtendedEventLinkageInfoEntry(ExtendedEventLinkageInfoEntry&) = delete;
            ExtendedEventLinkageInfoEntry& operator=(ExtendedEventLinkageInfoEntry&) = delete;

            ExtendedEventLinkageInfoEntry(ExtendedEventLinkageInfoEntry&&) = default;
            ExtendedEventLinkageInfoEntry& operator=(ExtendedEventLinkageInfoEntry&&) = default;
            ~ExtendedEventLinkageInfoEntry() = default;

            void print(std::size_t indent_level) const override;
            std::string deduceLinkType() const;
            std::string deduceTargetIdType() const;
        };

        ExtendedEventLinkageInfo(const std::uint8_t* start);

        ExtendedEventLinkageInfo() = delete;
        ExtendedEventLinkageInfo(ExtendedEventLinkageInfo&) = delete;
        ExtendedEventLinkageInfo& operator=(ExtendedEventLinkageInfo&) = delete;

        ExtendedEventLinkageInfo(ExtendedEventLinkageInfo&&) = default;
        ExtendedEventLinkageInfo& operator=(ExtendedEventLinkageInfo&&) = default;
        ~ExtendedEventLinkageInfo() = default;

        void print(std::size_t indent_level) const;
        std::uint8_t getLoopLength() const {
            return loop_length;
        }

    private:

        std::uint8_t loop_length;
        std::vector<ExtendedEventLinkageInfoEntry> loop;
    };
}



#endif // _EXTENDED_EVENT_LINKAGE_INFO_H