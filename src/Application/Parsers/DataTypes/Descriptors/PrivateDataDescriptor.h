#ifndef _PRIVATE_DATA_DESCRIPTOR_H
#define _PRIVATE_DATA_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

namespace Descriptors {

    class PrivateDataDescriptor: public Descriptor {

        std::uint32_t private_data_specifier;

    public:
        static const std::uint8_t tag;

        PrivateDataDescriptor(std::size_t len, const std::uint8_t* start)
            : Descriptor(len), private_data_specifier(
                (static_cast<std::uint32_t>(start[0]) << 24)
                | (static_cast<std::uint32_t>(start[1]) << 16)
                | (static_cast<std::uint32_t>(start[2]) << 8)
                | static_cast<std::uint32_t>(start[3])
            ) {}
        
        PrivateDataDescriptor(PrivateDataDescriptor&&) = default;
        PrivateDataDescriptor& operator=(PrivateDataDescriptor&&) = default;
        ~PrivateDataDescriptor() = default;

        void print(std::size_t indent_level) const override;
    };
} // namespace Descriptors



#endif // _PRIVATE_DATA_DESCRIPTOR_H