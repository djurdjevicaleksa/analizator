#ifndef _EVENT_LINKAGE_INFO_H
#define _EVENT_LINKAGE_INFO_H

#include <cstdint>


namespace SubStructs {

    class EventLinkageInfo {
    protected:
    
        std::uint16_t target_event_id;
        bool target_listed;
        bool event_simulcast;

    public:

        static constexpr std::uint8_t size = 3;

    public:

        EventLinkageInfo(const std::uint8_t* start);

        EventLinkageInfo() = delete;
        EventLinkageInfo(EventLinkageInfo&) = delete;
        EventLinkageInfo& operator=(EventLinkageInfo&) = delete;

        EventLinkageInfo(EventLinkageInfo&&) = default;
        EventLinkageInfo& operator=(EventLinkageInfo&&) = default;
        ~EventLinkageInfo() = default;
    };
}


#endif // _EVENT_LINKAGE_INFO_H