#include <stdexcept>
#include <memory>
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <iostream>

#include "src/Application/Parsers/DataTypes/Descriptors/Factory.h"
#include "src/Application/Parsers/DataTypes/Descriptors/UnknownDescriptor.h"

std::unordered_map<std::uint8_t, Factory::FactoryFn>& Factory::registry() {
    static std::unordered_map<std::uint8_t, Factory::FactoryFn> map_;
    return map_;
}

bool Factory::registerCreator(const std::uint8_t tag, const FactoryFn fn) {
    auto& reg = Factory::registry();
    if (reg.count(tag)) return false;
    reg.emplace(tag, std::move(fn));
    return true;
}

std::unique_ptr<Descriptors::Descriptor> Factory::create(std::uint8_t tag, std::size_t length, std::uint8_t* start) {
    auto& reg = Factory::registry();
    auto it = reg.find(tag);
    if (it != reg.end()) return (it->second)(length, start);
    
    return std::make_unique<Descriptors::UnknownDescriptor>(tag, length, start);
}