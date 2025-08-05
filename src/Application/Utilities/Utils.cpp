#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <cstring>
#include <optional>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "Utils.h"

void utils::printLine(const std::string& label, int indent, char border) {
    std::cout << std::string(indent * 4, ' ')
                << std::string(5, border) << " " << label << " "
                << std::string(5, border) << std::endl;
}

void utils::printByteArrayHex(const std::uint8_t* array, std::size_t length, std::size_t indent_level, std::size_t bytes_per_line) {

    if (bytes_per_line == 0) return;
    
    std::size_t counter = 0;
    
    for (std::size_t i = 0; i < length; i++) {
        if (counter % bytes_per_line == 0) std::cout << std::string(indent_level * 4, ' ');

        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << array[i] << ' ';
        counter++;

        if (counter % bytes_per_line == 0) std::cout << std::endl;
    }
}


// utils::mmap::mmap_t::mmap_t(const char* input_file) {

//     // Memory map and construct the object
//     int32_t ts_file_fd = ::open(input_file_name, O_RDONLY);
//     if (ts_file_fd < 0) {
//         std::cerr << "[TS] Failed to open the ts file." << std::endl;
//         this->data = nullptr;
//         this->length = 0;
//         return;
//     }

//     struct stat ts_file_stat;
//     if (fstat(static_cast<int>(ts_file_fd), &ts_file_stat) < 0) {
//         std::cerr << "[TS] Failed to get ts file size." << std::endl;
//         ::close(ts_file_fd);
//         this->data = nullptr;
//         this->length = 0;
//         return;
//     }

//     size_t ts_file_size = static_cast<size_t>(ts_file_stat.st_size);

//     void* mapped_ts = mmap(nullptr, ts_file_size, PROT_READ, MAP_PRIVATE, ts_file_fd, 0);
//     if (mapped_ts == MAP_FAILED) {
//         std::cerr << "[TS] Failed to memory map the ts file into process memory." << std::endl;
//         ::close(ts_file_fd);
//         return std::nullopt;
//     }
//     ::close(ts_file_fd);

// }

// std::optional<utils::mmap::mmap_t> utils::mmap::mmap_posix(const char* input_file_name) {
//     int32_t ts_file_fd = ::open(input_file_name, O_RDONLY);
//     if (ts_file_fd < 0) {
//         std::cerr << "[TS] Failed to open the ts file." << std::endl;
//         return std::nullopt;
//     }

//     struct stat ts_file_stat;
//     if (fstat(static_cast<int>(ts_file_fd), &ts_file_stat) < 0) {
//         std::cerr << "[TS] Failed to get ts file size." << std::endl;
//         ::close(ts_file_fd);
//         return std::nullopt;
//     }

//     size_t ts_file_size = static_cast<size_t>(ts_file_stat.st_size);

//     void* mapped_ts = mmap(nullptr, ts_file_size, PROT_READ, MAP_PRIVATE, ts_file_fd, 0);
//     if (mapped_ts == MAP_FAILED) {
//         std::cerr << "[TS] Failed to memory map the ts file into process memory." << std::endl;
//         ::close(ts_file_fd);
//         return std::nullopt;
//     }
//     ::close(ts_file_fd);

//     return map<const uint8_t*>(mapped_ts, ts_file_size);
// }
