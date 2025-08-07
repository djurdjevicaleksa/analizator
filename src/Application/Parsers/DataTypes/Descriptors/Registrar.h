#ifndef _REGISTRAR_H
#define _REGISTRAR_H

#include <cstdint>
#include <memory>

#include "src/Application/Parsers/DataTypes/Descriptors/Factory.h"

template<typename DerivedDescriptor>
struct Registrar {
    Registrar(const std::uint8_t tag) {
        Factory::registerCreator(tag,
            [](std::size_t length, std::uint8_t* start) {
                return std::make_unique<DerivedDescriptor>(length, start);
            }
        );
    }
};

#endif // _REGISTRAR_H