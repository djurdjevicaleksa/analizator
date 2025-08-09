#include <cstdint>
#include <cstddef>

#include "PrivateDataDescriptor.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Registrar.h"
#include "src/Application/Utilities/Utils.h"

const std::uint8_t Descriptors::PrivateDataDescriptor::tag = 0x5F;
static Registrar<Descriptors::PrivateDataDescriptor> private_data_descriptor_registrar{ Descriptors::PrivateDataDescriptor::tag };

void Descriptors::PrivateDataDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Private data descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    utils::printDataPoint("Private data specifier", this->private_data_specifier, indent_level);
    utils::printLine("Private data descriptor", indent_level, '=');
}