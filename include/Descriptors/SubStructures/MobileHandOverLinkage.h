#ifndef _MOBILE_HAND_OVER_LINKAGE_H
#define _MOBILE_HAND_OVER_LINKAGE_H

#include <cstdint>
#include <cstddef>
#include <optional>
#include <string>

namespace SubStructs {

    class MobileHandOverLinkage {

        std::uint8_t hand_over_type;
        bool origin_type;
        std::optional<std::uint16_t> network_id;
        std::optional<std::uint16_t> initial_service_id;

    public:

        std::uint8_t* end_pointer;

    public:

        MobileHandOverLinkage(const std::uint8_t* start);

        MobileHandOverLinkage() = delete;
        MobileHandOverLinkage(MobileHandOverLinkage&) = delete;
        MobileHandOverLinkage& operator=(MobileHandOverLinkage&) = delete;

        MobileHandOverLinkage(MobileHandOverLinkage&&) = default;
        MobileHandOverLinkage& operator=(MobileHandOverLinkage&&) = default;
        ~MobileHandOverLinkage() = default;

        std::string deduceHandOverType() const;
        std::string deduceOriginType() const;
    };
}


#endif // _MOBILE_HAND_OVER_LINKAGE_H