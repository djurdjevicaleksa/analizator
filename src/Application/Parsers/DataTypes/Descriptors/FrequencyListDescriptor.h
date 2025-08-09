#ifndef _FREQUENCY_LIST_DESCRIPTOR_H
#define _FREQUENCY_LIST_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>
#include <vector>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

namespace Descriptors {

    class FrequencyListDescriptor: public Descriptor {

        std::uint8_t coding_type;
        std::vector<std::uint32_t> centre_frequencies;

    public:
        static const std::uint8_t tag;

        FrequencyListDescriptor(std::size_t len, const std::uint8_t* start);

        FrequencyListDescriptor(FrequencyListDescriptor&&) = default;
        FrequencyListDescriptor& operator=(FrequencyListDescriptor&&) = default;
        ~FrequencyListDescriptor() = default;

        void print(std::size_t indent_level) const override;
    };

} // namespace Descriptors





#endif // _FREQUENCY_LIST_DESCRIPTOR_H