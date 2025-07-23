#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <iostream>
#include <iomanip>
#include <cstdint>

namespace utils {

    template<typename T>
    void printHex(const std::string& label, T value, int indent = 1) {
        std::cout << std::string(indent * 4, ' ')
                  << label << ": 0x"
                  << std::hex << std::uppercase << std::setfill('0')
                  << std::setw(sizeof(T) * 2)
                  << static_cast<uint64_t>(value)
                  << std::dec << std::endl;
    }

    void printLine(const std::string& label, int indent = 1, char border = '=');

}

#endif // _UTILS_H