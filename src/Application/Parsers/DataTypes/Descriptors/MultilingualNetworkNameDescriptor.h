#ifndef _MULTILINGUAL_NETWORK_NAME_DESCRIPTOR_H
#define _MULTILINGUAL_NETWORK_NAME_DESCRIPTOR_H

#include <cstdint>
#include <vector>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

class MultilingualNetworkNameDescriptor: public Descriptors::Descriptor {
public:
    struct MultilingualNetworkName {

        std::uint32_t ISO_639_language_code;
        std::uint8_t network_name_length;
        std::vector<std::uint8_t> network_name_char_array;
        
        const std::uint8_t* end_pointer;
        
        MultilingualNetworkName(const std::uint8_t* start);

        MultilingualNetworkName() = delete;
        MultilingualNetworkName(const MultilingualNetworkName&) = delete;
        MultilingualNetworkName& operator=(const MultilingualNetworkName&) = delete;

        MultilingualNetworkName(MultilingualNetworkName&&) = default;
        MultilingualNetworkName& operator=(MultilingualNetworkName&&) = default;
        ~MultilingualNetworkName() = default;
    };


    MultilingualNetworkNameDescriptor(std::size_t length, const std::uint8_t* start);
    
    MultilingualNetworkNameDescriptor() = delete;
    MultilingualNetworkNameDescriptor(const MultilingualNetworkName&) = delete;
    MultilingualNetworkNameDescriptor& operator=(const MultilingualNetworkNameDescriptor&) = delete;
    
    MultilingualNetworkNameDescriptor(MultilingualNetworkNameDescriptor&&) = default;
    MultilingualNetworkNameDescriptor& operator=(MultilingualNetworkNameDescriptor&&) = default;
    ~MultilingualNetworkNameDescriptor() = default;

private:

    std::vector<MultilingualNetworkName> network_names;
};



#endif // _MULTILINGUAL_NETWORK_NAME_DESCRIPTOR_H