#ifndef _DESCRIPTOR_FALLBACK_H
#define _DESCRIPTOR_FALLBACK_H

#include "src/Application/Parsers/DataTypes/Descriptors/UnknownDescriptor.h"

namespace Descriptors {
    
    template<int ID>
    struct DerivedDescriptorFromTag {
        using type = Descriptors::UnknownDescriptor;
    };
} // namespace Descriptors

#endif // _DESCRIPTOR_FALLBACK_H