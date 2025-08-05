#include <cstdint>
#include <cstddef>
#include <optional>

#include "MobileHandOverLinkage.h"
#include "src/Application/Utilities/Utils.h"

SubStructs::MobileHandOverLinkage::MobileHandOverLinkage(const std::uint8_t* start) {
    this->hand_over_type = (start[0] >> 4) && 0b1111;
    this->origin_type = start[0] & 0b1;
    this->end_pointer = const_cast<std::uint8_t*>(&start[1]);

    std::uint8_t* it = const_cast<std::uint8_t*>(&start[1]);
    
    if ((this->hand_over_type > 0) && (this->hand_over_type < 4)) {
        this->network_id = std::optional<std::uint16_t>((it[1] << 8) | it[2]);
        it += 2;
        this->end_pointer += 2;
    }
    else this->network_id = std::nullopt;

    if (this->origin_type == 0) {
        this->initial_service_id = std::optional<std::uint8_t>((it[0] << 8) | it[1]);
        it += 2;
        this->end_pointer += 2;
    }
    else this->initial_service_id = std::nullopt;
}

std::string SubStructs::MobileHandOverLinkage::deduceHandOverType() const {
    switch(this->hand_over_type) {
        case 1: return "DVB hand-over to an identical service in a neighbouring country";
        case 2: return "DVB hand-over to a local variation of the same service";
        case 3: return "DVB hand-over to an associated service";
        default: return "Reserved for future use"; 
    }
}

std::string SubStructs::MobileHandOverLinkage::deduceOriginType() const {
    return this->origin_type == 0? "NIT" : "SDT";
}

void SubStructs::MobileHandOverLinkage::print(std::size_t indent_level) const {
    utils::printLine("Mobile hand-over linkage", indent_level, '=');
    utils::printDataPoint("Hand-over type", this->hand_over_type, indent_level, this->deduceHandOverType());
    utils::printDataPoint("Origin type", this->origin_type, indent_level, this->deduceOriginType());
    if (this->network_id.has_value()) utils::printDataPoint("Network ID", this->network_id.value(), indent_level);
    if (this->initial_service_id.has_value()) utils::printDataPoint("Initial service ID", this->initial_service_id.value(), indent_level);
    utils::printLine("Mobile hand-over linkage", indent_level, '=');
}