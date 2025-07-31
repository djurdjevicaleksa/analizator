#include <cstdint>

#include "EventLinkageInfo.h"

SubStructs::EventLinkageInfo::EventLinkageInfo(const std::uint8_t* start) {
    this->target_event_id = (start[0] << 8) | start[1];
    this->target_listed = (start[2] >> 7) & 0b1;
    this->event_simulcast = (start[2] >> 6) & 0b1;
}