#include <cstddef>
#include <cstdint>

#include "ServiceListDescriptor.h"
#include "Utils.h"

Descriptors::ServiceListDescriptor::ServiceListDescriptor(std::uint8_t len, const std::uint8_t* list_start) : Descriptors::Descriptor(len) {
            
    const std::uint8_t* list_end = list_start + len;
    this->service_list.reserve((std::uint16_t)(list_end - list_start) / service_identifier_size + 1);
    
    std::uint8_t* it = const_cast<std::uint8_t*>(list_start);

    while (it < list_end) {
        if (it + 2 >= list_end) throw std::runtime_error("[ServiceListDescriptor] Failed to parse input: Malformed packet.");

        this->service_list.emplace_back((static_cast<std::uint16_t>(it[0]) << 8) | it[1], it[2]);
        it += this->service_identifier_size;
    }
}

void Descriptors::ServiceListDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Service list descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    std::cout << std::endl;
    utils::printLine("Service identifiers", indent_level + 1, '=');
    for (std::size_t i = 0; i < this->service_list.size(); i++) this->service_list[i].print(indent_level + 1);
    std::cout << std::endl;
    utils::printLine("Service list descriptor", indent_level, '=');
}

void Descriptors::ServiceListDescriptor::ServiceIdentifier::print(std::size_t indent_level) const {
    utils::printLine("Service identifier", indent_level, '-');
    utils::printDataPoint("Service ID", this->service_id, indent_level);
    utils::printDataPoint("Service type", this->service_type, indent_level);
}