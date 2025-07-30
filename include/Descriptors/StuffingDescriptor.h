#ifndef _STUFFING_DESCRIPTOR_H
#define _STUFFING_DESCRIPTOR_H

#include <memory>
#include <cstdint>
#include <cstddef>

#include "Descriptor.h"
#include "DescriptorTemplatization.h"

/*
    STD: The stuffing descriptor provides a means of invalidating previously
    coded descriptors or inserting dummy descriptors for table stuffing.
*/

namespace Descriptors {

    class StuffingDescriptor: Descriptor {

        std::unique_ptr<std::uint8_t[]> stuffing_bytes;

    public:
        static constexpr std::uint8_t tag = 0x42;
        
        StuffingDescriptor(std::size_t len, const std::uint8_t* start)
            : Descriptor(len), stuffing_bytes(std::make_unique<std::uint8_t[]>(len)) {
                
            std::copy(start, start + len, this->stuffing_bytes.get());
        }

        StuffingDescriptor(StuffingDescriptor&&) = default;
        StuffingDescriptor& operator=(StuffingDescriptor&&) = default;
        ~StuffingDescriptor() = default;

        void print(std::size_t) const override;
    };
}

template<> struct DerivedDescriptorFromTag<Descriptors::StuffingDescriptor::tag> { using type = Descriptors::StuffingDescriptor; };

#endif // _STUFFING_DESCRIPTOR_H