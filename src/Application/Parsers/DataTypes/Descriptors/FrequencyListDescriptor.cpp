#include <cstdint>
#include <cstddef>
#include <vector>

#include "src/Application/Parsers/DataTypes/Descriptors/FrequencyListDescriptor.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Registrar.h"
#include "src/Application/Utilities/Utils.h"

const std::uint8_t Descriptors::FrequencyListDescriptor::tag = 0x62;
static Registrar<Descriptors::FrequencyListDescriptor> frequency_list_descriptor_registrar{ Descriptors::FrequencyListDescriptor::tag };

Descriptors::FrequencyListDescriptor::FrequencyListDescriptor(std::size_t len, const std::uint8_t* start)
    : Descriptor(len) {

    this->coding_type = start[0] & 0b11;

    std::uint8_t* moving_ptr = const_cast<std::uint8_t*>(&start[1]);
    const std::uint8_t* frequencies_end = start + len;

    while (moving_ptr < frequencies_end) {

        this->centre_frequencies.emplace_back(
            (static_cast<std::uint32_t>(moving_ptr[0]) << 24)
            | (static_cast<std::uint32_t>(moving_ptr[1]) << 16)
            | (static_cast<std::uint32_t>(moving_ptr[2]) << 8)
            | moving_ptr[1]
        );

        moving_ptr += 4;
    }
}

void Descriptors::FrequencyListDescriptor::print(std::size_t indent_level) const {
    utils::printLine("Frequency list descriptor", indent_level, '=');
    utils::printDataPoint("Descriptor tag", this->tag, indent_level);
    utils::printDataPoint("Descriptor length", this->length, indent_level);
    utils::printDataPoint("Coding type", this->coding_type, indent_level);
    for (std::size_t i = 0; i < this->centre_frequencies.size(); i++)
        utils::printDataPoint("Frequency " + std::to_string(i), this->centre_frequencies[i], indent_level + 1);
    utils::printLine("Frequency list descriptor", indent_level, '=');
}