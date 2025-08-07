#ifndef _FACTORY_H
#define _FACTORY_H

#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>

#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

class Factory {
public:
    using FactoryFn = std::function<std::unique_ptr<Descriptors::Descriptor>(std::size_t, std::uint8_t*)>;

    static std::unique_ptr<Descriptors::Descriptor> create(std::uint8_t tag, std::size_t length, std::uint8_t* start);
    static bool registerCreator(const std::uint8_t tag, FactoryFn fn);

private:
    static std::unordered_map<std::uint8_t, FactoryFn>& registry();
};

#endif // _FACTORY_H