#include <cstdint>
#include <vector>

#include "src/Application/Parsers/DataTypes/Descriptors/MultilingualNetworkNameDescriptor.h"


MultilingualNetworkNameDescriptor::MultilingualNetworkName::MultilingualNetworkName(const std::uint8_t* start) :
    ISO_639_language_code(static_cast<uint32_t>(start[0] << 16) | (start[1] << 8) | start[2]),
    network_name_length(start[3]),
    network_name_char_array(&start[4], &start[4] + network_name_length), end_pointer(&start[4] + network_name_length) {}

MultilingualNetworkNameDescriptor::MultilingualNetworkNameDescriptor(std::size_t length, const std::uint8_t* start) : Descriptor(length) {

    if (!length) return;

    std::uint8_t* moving_ptr = const_cast<std::uint8_t*>(start);
    const std::uint8_t* descriptor_end = start + length;

    while (moving_ptr < descriptor_end) {

        this->network_names.emplace_back(moving_ptr);
        moving_ptr = const_cast<std::uint8_t*>(this->network_names.rbegin()->end_pointer);
    }
}