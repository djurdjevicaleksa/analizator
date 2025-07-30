#ifndef _SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_H
#define _SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>
#include <string>

#include "Descriptor.h"
#include "DescriptorTemplatization.h"

namespace Descriptors {

    class SatelliteDeliverySystemDescriptor: Descriptor {
        
        std::uint32_t frequency;
        std::uint16_t orbital_position;
        bool west_east_flag;
        std::uint8_t polarization;
        std::uint8_t roll_off;
        bool modulation_system;
        std::uint8_t modulation_type;
        std::uint32_t symbol_rate;
        std::uint8_t fec_inner;

    public:
        static constexpr std::uint8_t tag = 0x43;

        SatelliteDeliverySystemDescriptor(std::size_t length, const std::uint8_t* start);

        SatelliteDeliverySystemDescriptor(SatelliteDeliverySystemDescriptor&&) = default;
        SatelliteDeliverySystemDescriptor& operator=(SatelliteDeliverySystemDescriptor&&) = default;
        ~SatelliteDeliverySystemDescriptor() = default;

        void print(std::size_t indent_level) const override;
        double deduceFrequencyGHz() const;
        float deduceOrbitalPositionInDegrees() const;
        char deduceWestEast() const;
        std::string deducePolarization() const;
        float deduceRollOff() const;
        std::string deduceModulationSystem() const;
        std::string deduceModulationType() const;
        double deduceSymbolRate() const;
        std::string deduceFECScheme() const;
    };
}

template<> struct DerivedDescriptorFromTag<Descriptors::SatelliteDeliverySystemDescriptor::tag> { using type = Descriptors::SatelliteDeliverySystemDescriptor; };

#endif // _SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR_H