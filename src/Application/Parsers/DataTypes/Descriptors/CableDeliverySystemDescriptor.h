#ifndef _CABLE_DELIVERY_SYSTEM_DESCRIPTOR_H
#define _CABLE_DELIVERY_SYSTEM_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>
#include <string>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

namespace Descriptors {

    class CableDeliverySystemDescriptor: public Descriptor {

        std::uint32_t frequency;
        std::uint8_t fec_outer;
        std::uint8_t modulation;
        std::uint32_t symbol_rate;
        std::uint8_t fec_inner;

    public:

        static const std::uint8_t tag;

        CableDeliverySystemDescriptor(std::size_t length, const std::uint8_t* start);

        CableDeliverySystemDescriptor(CableDeliverySystemDescriptor&&) = default;
        CableDeliverySystemDescriptor& operator=(CableDeliverySystemDescriptor&&) = default;
        ~CableDeliverySystemDescriptor() = default;

        void print(std::size_t indent_level) const override;

        double deduceFrequency() const;
        std::string deduceFECOuterScheme() const;
        std::string deduceModulationScheme() const;
        double deduceSymbolRate() const;
        std::string deduceFECInnerScheme() const;
    };
}


#endif // _CABLE_DELIVERY_SYSTEM_DESCRIPTOR_H