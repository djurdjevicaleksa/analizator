#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <cstdint>
#include <vector>
#include <string>

struct Descriptor {
        uint8_t tag;
        uint8_t length;
        std::vector<uint8_t> data;

        Descriptor(uint8_t t, uint8_t len, const uint8_t* start) : tag(t), length(len), data(start, start + len) {}
        void print(std::string (*tagToNameMethod)(uint8_t)) const;
    };

#endif // _DESCRIPTOR_H