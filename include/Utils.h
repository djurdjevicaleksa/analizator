#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <cstdint>
#include <optional>

// #ifdef _WIN32
//     #include <windows.h>
// #else
//     #include <fcntl.h>
//     #include <unistd.h>
//     #include <sys/stat.h>
//     #include <sys/mman.h>
// #endif

// namespace utils {

    
//     namespace mmap {

//         template<typename T>
//         struct mmap_t {
//             T* data;
//             size_t size;
    
//             // POSIX allows closing the fd immediately after mmapping, but Windows doesn't specify
//             #ifdef _WIN32
//                 HANDLE file_handle;
//                 HANDLE mapping_handle;
//             #endif
            
//             mmap_t(const char*);
            
//             // Disable copy
//             mmap_t(const mmap_t&) = delete;
//             mmap_t& operator=(const mmap_t&) = delete;

//             // Enable move
//             mmap_t(const mmap_t&& other) noexcept {
//                 this->data = other.data;
//                 this->size = other.size;
//                 other.data = nullptr;
//                 other.size = 0;

//                 #ifdef _WIN32
//                     this->file_handle = other.file_handle;
//                     this->mapping_handle = other.mapping_handle;
//                 #endif
//             }

//             mmap_t& operator=(mmap_t&& other) {
//                 if (this != &other) {
//                     this->~mmap_t();
//                     new (this) mmap_t(std::move(other));
//                 }
//                 return *this;
//             }

//             ~mmap_t();

//             private:
            
//         };
        
//         template<typename T>
//         mmap_t<T>::mmap_t(const char* input_file_name) {

//             #ifdef _WIN32

//             #else
//                 // Memory map and initialize the object
//                 int32_t ts_file_fd = ::open(input_file_name, O_RDONLY);
//                 if (ts_file_fd < 0) {
//                     std::cerr << "[TS] Failed to open the ts file." << std::endl;
//                     this->data = nullptr;
//                     this->length = 0;
//                     return;
//                 }

//                 struct stat ts_file_stat;
//                 if (fstat(static_cast<int>(ts_file_fd), &ts_file_stat) < 0) {
//                     std::cerr << "[TS] Failed to get ts file size." << std::endl;
//                     ::close(ts_file_fd);
//                     this->data = nullptr;
//                     this->length = 0;
//                     return;
//                 }

//                 size_t ts_file_size = static_cast<size_t>(ts_file_stat.st_size);

//                 void* mapped_ts = mmap(nullptr, ts_file_size, PROT_READ, MAP_PRIVATE, ts_file_fd, 0);
//                 if (mapped_ts == MAP_FAILED) {
//                     std::cerr << "[TS] Failed to memory map the ts file into process memory." << std::endl;
//                     ::close(ts_file_fd);
//                     this->data = nullptr;
//                     this->length = 0;
//                     return;
//                 }
//                 ::close(ts_file_fd);

//                 this->data = static_cast<T*>(mapped_ts);
//                 this->size = ts_file_size;
//             #endif
//         }
        

//     }


// }

namespace utils {
    template<typename T>
    void printDataPoint(const std::string& label, T value, int indent = 1) {
        std::cout << std::string(indent * 4, ' ')
                  << label << ": 0x"
                  << std::hex << std::uppercase << std::setfill('0')
                  << std::setw(sizeof(T) * 2)
                  << static_cast<uint64_t>(value)
                  << std::dec << std::endl;
    }

    void printDataPoint(const std::string& label, const std::string& value, int indent = 1) {
        std::cout << std::string(indent * 4, ' ') << label << ": " << value << std::endl;
    }
                
    void printLine(const std::string& label, int indent = 1, char border = '=');

    void printByteArrayHex(const std::string&, const std::uint8_t*, std::size_t);
}


#endif // _UTILS_H