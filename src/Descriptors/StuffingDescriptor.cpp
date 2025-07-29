#include <cstddef>

#include "StuffingDescriptor.h"
#include "Utils.h"

void Descriptors::StuffingDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Stuffing descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    utils::printDataPoint("Stuffing bytes", std::string(reinterpret_cast<char*>(this->stuffing_bytes.get()), this->length), indent_level);
    utils::printLine("Stuffing descriptor", indent_level, '=');
}