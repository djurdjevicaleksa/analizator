#include <cstdint>

#include "EventLinkageInfo.h"
#include "src/Application/Utilities/Utils.h"

SubStructs::EventLinkageInfo::EventLinkageInfo(const std::uint8_t* start) {
    this->target_event_id = (start[0] << 8) | start[1];
    this->target_listed = (start[2] >> 7) & 0b1;
    this->event_simulcast = (start[2] >> 6) & 0b1;
}

void SubStructs::EventLinkageInfo::print(std::size_t indent_level) const {
    utils::printLine("Event linkage info", indent_level, '=');
    utils::printDataPoint("Target event ID", this->target_event_id, indent_level);
    utils::printDataPoint("Target listed", this->target_listed, indent_level);
    utils::printDataPoint("Event simulcast", this->event_simulcast, indent_level);
    utils::printLine("Event linkage info", indent_level, '=');
}