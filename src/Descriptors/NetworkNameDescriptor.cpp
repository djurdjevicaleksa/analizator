#include <cstddef>

#include "NetworkNameDescriptor.h"
#include "Utils.h"

void Descriptors::NetworkNameDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Network name descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    utils::printDataPoint("Network name", std::string(reinterpret_cast<char*>(this->data.get()), this->length), indent_level);
    utils::printLine("Network name descriptor", indent_level, '=');
}