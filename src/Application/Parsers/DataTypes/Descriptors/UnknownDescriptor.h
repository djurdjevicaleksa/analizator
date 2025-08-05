#ifndef _UNKNOWN_DESCRIPTOR_H
#define _UNKNOWN_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>

#include "src/Application/Parsers/DataTypes/Descriptors/DescriptorFallback.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

namespace Descriptors {

    class UnknownDescriptor: public Descriptor {
    public:
        std::uint8_t tag;

        UnknownDescriptor(std::uint8_t tag, std::size_t length, const std::uint8_t* data):
            Descriptor(length), tag(tag) {}

        UnknownDescriptor(const UnknownDescriptor&) = default;
        UnknownDescriptor& operator=(UnknownDescriptor&&) = default;
        ~UnknownDescriptor() = default;
    };
} // namespace Descriptors

#endif // _UNKNOWN_DESCRIPTOR_H