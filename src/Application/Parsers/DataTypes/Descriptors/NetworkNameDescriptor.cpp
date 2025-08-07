#include <cstddef>
#include <cstdint>

#include "src/Application/Parsers/DataTypes/Descriptors/NetworkNameDescriptor.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Registrar.h"
#include "src/Application/Utilities/Utils.h"

const std::uint8_t Descriptors::NetworkNameDescriptor::tag = 0x40;
static Registrar<Descriptors::NetworkNameDescriptor> network_descriptor_registrar{ Descriptors::NetworkNameDescriptor::tag };

void Descriptors::NetworkNameDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Network name descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    utils::printDataPoint("Network name", std::string(reinterpret_cast<char*>(this->data.get()), this->length), indent_level);
    utils::printLine("Network name descriptor", indent_level, '=');
}