#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <cstdint>

namespace utils {
    template<typename T>
    inline void printDataPoint(const std::string& label, T value, int indent = 1, std::string explanation = "") {
        std::cout << std::string(indent * 4, ' ')
                  << label << ": 0x"
                  << std::hex << std::uppercase << std::setfill('0')
                  << std::setw(sizeof(T) * 2)
                  << static_cast<uint64_t>(value)
                  << std::dec << "(" << explanation << ")" << std::endl;
    }

    inline void printDataPoint(const std::string& label, const std::string& value, int indent = 1, std::string explanation = "") {
        std::cout << std::string(indent * 4, ' ') << label << ": " << value << " (" << explanation << ")" << std::endl;
    }

    inline void printDataPoint(const std::string& label, bool value, int indent, std::string explanation = "") {
        std::cout << std::string(indent * 4, ' ') << label << " : " << (value? "yes" : "no") << " (" << explanation << ")" << std::endl; 
    }
                
    void printLine(const std::string& label, int indent = 1, char border = '=');

    void printByteArrayHex(const std::uint8_t* array, std::size_t length, std::size_t indent_level, std::size_t bytes_per_line);

    inline std::uint32_t readMSB32(const std::uint8_t* start) {
        return (static_cast<std::uint32_t>(start[0]) << 24) | (static_cast<std::uint32_t>(start[1]) << 16)
                | (static_cast<std::uint32_t>(start[2]) << 8) | static_cast<std::uint32_t>(start[3]);
    }

    inline std::uint16_t readMSB16(const std::uint8_t* start) {
        return (static_cast<std::uint16_t>(start[0]) << 8) | static_cast<std::uint16_t>(start[1]);
    }
}


#endif // _UTILS_H