#ifndef _NETWORK_NAME_DESCRIPTOR_H
#define _NETWORK_NAME_DESCRIPTOR_H

#include <memory>
#include <string>
#include <cstddef>
#include <cstdint>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

/*
    STD: The network name descriptor provides the network name in text form.
*/

namespace Descriptors {
    
    class NetworkNameDescriptor: public Descriptor {
    
        std::unique_ptr<std::uint8_t[]> data;
        
    public:
        static const std::uint8_t tag;
        
        NetworkNameDescriptor(std::size_t len, const std::uint8_t* start)
            : Descriptor(len), data(std::make_unique<std::uint8_t[]>(len)) {
            
            std::copy(start, start + len, this->data.get());
        }
        
        NetworkNameDescriptor(NetworkNameDescriptor&&) = default;
        NetworkNameDescriptor& operator=(NetworkNameDescriptor&&) = default;
        ~NetworkNameDescriptor() = default;
    
        void print(std::size_t indent_level) const override;
    };
}


#endif // _NETWORK_NAME_DESCRIPTOR_H