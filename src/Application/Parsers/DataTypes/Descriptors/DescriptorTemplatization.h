#ifndef _DESCRIPTOR1_H
#define _DESCRIPTOR1_H

#include <vector>
#include <string>
#include <memory>

#include "src/Application/Parsers/DataTypes/Descriptors/DescriptorFallback.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

namespace Descriptors {
    
    template<int tag>
    struct DerivedDescriptorFromTag;
    
    template<int ID>
    std::unique_ptr<Descriptor> create(const std::uint8_t length, const std::uint8_t* start) {
        return std::unique_ptr<Descriptor>(new typename DerivedDescriptorFromTag<ID>::type(length, start));
    }

} // namespace Descriptors


#endif // _DESCRIPTOR1_H