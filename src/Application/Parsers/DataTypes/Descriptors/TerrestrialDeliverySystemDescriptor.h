#ifndef _TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_H
#define _TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>
#include <string>

#include "src/Application/Parsers/DataTypes/Descriptors/DescriptorFallback.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h" 

namespace Descriptors {

    class TerrestrialDeliverySystemDescriptor: public Descriptor {

        std::uint32_t centre_frequency;
        std::uint8_t bandwidth;
        bool priority;
        bool time_slicing_indicator;
        bool mpe_fec_indicator;
        std::uint8_t constellation;
        std::uint8_t hierarchy_information;
        std::uint8_t code_rate_hp_stream;
        std::uint8_t code_rate_lp_stream;
        std::uint8_t guard_interval;
        std::uint8_t transmission_mode;
        bool other_frequency_flag;

    public:
        static constexpr std::uint8_t tag = 0x5A;

        TerrestrialDeliverySystemDescriptor(std::size_t len, const std::uint8_t* start);

        TerrestrialDeliverySystemDescriptor(TerrestrialDeliverySystemDescriptor&&) = default;
        TerrestrialDeliverySystemDescriptor& operator=(TerrestrialDeliverySystemDescriptor&&) = default;
        ~TerrestrialDeliverySystemDescriptor() = default;        

        void print(std::size_t indent_level) const;

        inline std::uint64_t deduceCentreFrequencyHz() const {
            return static_cast<std::uint64_t>(this->centre_frequency) * 10;
        }

        inline float deduceCentreFrequencyKHz() const {
            return static_cast<float>(this->centre_frequency) / 100.0;
        }

        inline double deduceCentreFrequencyMHz() const {
            return static_cast<double>(this->centre_frequency) / 100'000.0;
        }

        inline double deduceCentreFrequencyGHz() const {
            return static_cast<double>(this->centre_frequency) / 100'000'000.0;
        }

        std::string deduceBandwidth() const;

        inline std::string deducePriority() const {
            return this->priority == 0? "High priority" : "Low priority";
        }

        std::string deduceConstellationPattern() const;
        std::string deduceHierarchyInformation() const;
        std::string deduceCodeRate() const;
        std::string deduceGuardInterval() const;
        std::string deduceTransmissionMode() const;
    };
    
    template<>
    struct DerivedDescriptorFromTag<TerrestrialDeliverySystemDescriptor::tag> {
        using type = TerrestrialDeliverySystemDescriptor;
    };
}


#endif // _TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_H