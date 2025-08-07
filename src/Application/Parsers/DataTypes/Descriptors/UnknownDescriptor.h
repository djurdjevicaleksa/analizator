#ifndef _UNKNOWN_DESCRIPTOR_H
#define _UNKNOWN_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"
#include "src/Application/Utilities/Utils.h"

namespace Descriptors {

    class UnknownDescriptor: public Descriptor {
    public:
        std::uint8_t tag;

        UnknownDescriptor(std::uint8_t tag, std::size_t length, const std::uint8_t* data):
            Descriptor(length), tag(tag) {}

        UnknownDescriptor(const UnknownDescriptor&) = default;
        UnknownDescriptor& operator=(UnknownDescriptor&&) = default;
        ~UnknownDescriptor() override = default;

        void print(std::size_t indent_level) const override {
            utils::printLine("Unknown descriptor", indent_level, '=');
            utils::printDataPoint("Descriptor tag", this->tag, indent_level);
            utils::printDataPoint("Descriptor length", this->length, indent_level);
            utils::printLine("Unknown descriptor", indent_level, '=');
        }
    };
} // namespace Descriptors

#endif // _UNKNOWN_DESCRIPTOR_H