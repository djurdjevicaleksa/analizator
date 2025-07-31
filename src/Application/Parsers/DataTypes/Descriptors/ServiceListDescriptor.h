#ifndef _SERVICE_LIST_DESCRIPTOR_H
#define _SERVICE_LIST_DESCRIPTOR_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <cassert>
#include <iostream>

#include "Descriptor.h"
#include "DescriptorTemplatization.h"

/*
    STD: The service list descriptor provides a means of listing the services by service_id and service type.
*/

namespace Descriptors {
    
    class ServiceListDescriptor : Descriptor {
        
        struct __attribute__((packed)) ServiceIdentifier {
            std::uint16_t service_id;
            std::uint8_t service_type;
            
            ServiceIdentifier(std::uint16_t sid, std::uint8_t stype)
                : service_id(sid), service_type(stype) {}
            
            ServiceIdentifier(ServiceIdentifier&) = delete;
            ServiceIdentifier& operator=(ServiceIdentifier&) = delete;

            void print(std::size_t) const;
        };

        static constexpr uint8_t service_identifier_size = sizeof(ServiceIdentifier);

        std::vector<ServiceIdentifier> service_list;
    
    public:
        static constexpr uint8_t tag = 0x41;

        ServiceListDescriptor(uint8_t len, const uint8_t* list_start);

        ServiceListDescriptor(ServiceListDescriptor&&) = default;
        ServiceListDescriptor& operator=(ServiceListDescriptor&&) = default;
        ~ServiceListDescriptor() = default;
        
        void print(std::size_t) const override;
    };
}

template<> struct DerivedDescriptorFromTag<Descriptors::ServiceListDescriptor::tag> {using type = Descriptors::ServiceListDescriptor; };

#endif // _SERVICE_LIST_DESCRIPTOR_H