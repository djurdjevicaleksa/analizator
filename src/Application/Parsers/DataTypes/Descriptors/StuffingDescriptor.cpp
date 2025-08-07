#include <cstddef>
#include <cstdint>

#include "src/Application/Parsers/DataTypes/Descriptors/StuffingDescriptor.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Registrar.h"
#include "src/Application/Utilities/Utils.h"

const std::uint8_t Descriptors::StuffingDescriptor::tag = 0x42;
static Registrar<Descriptors::StuffingDescriptor> stuffing_descriptor_registrar{ Descriptors::StuffingDescriptor::tag };

void Descriptors::StuffingDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Stuffing descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    utils::printLine("Stuffing bytes", indent_level, '-');
    utils::printByteArrayHex(this->stuffing_bytes.get(), this->length, indent_level + 1, 8);
    utils::printLine("Stuffing descriptor", indent_level, '=');
}