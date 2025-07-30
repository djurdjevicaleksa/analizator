#ifndef _DESCRIPTOR1_H
#define _DESCRIPTOR1_H

#include <vector>
#include <string>
#include <memory>

#include "Descriptor.h"
#include "Utils.h"

// struct Descriptor {
//     uint8_t tag;
//     std::size_t length;
//     std::vector<uint8_t> data;

//     Descriptor(uint8_t t, std::size_t len, const uint8_t* start) : tag(t), length(len), data(start, start + len) {}
//     void print(std::string (*tagToNameMethod)(uint8_t)) const;
// };

// ======================================

template<int tag>
struct DerivedDescriptorFromTag;

template<int ID, typename... Args>
std::unique_ptr<Descriptors::Descriptor> create(Args&&... args) {
    using T = typename DerivedDescriptorFromTag<ID>::type;
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif // _DESCRIPTOR1_H