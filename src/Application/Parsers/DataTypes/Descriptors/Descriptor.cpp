#include <cstdint>

#include "Descriptor.h"
#include "Utils.h"

// void Descriptor::print(std::string (*tagToNameMethod)(uint8_t)) const {
    
//     utils::printLine("DESCRIPTOR", 2, '-');
//     std::cout << std::string(8, ' ') << "descriptor_tag: 0x" << std::hex
//             << std::uppercase << std::setfill('0') << std::setw(sizeof(this->tag) * 2)
//             << static_cast<uint64_t>(this->tag) << std::dec << " => "
//             << tagToNameMethod(this->tag) << std::endl;
//     utils::printDataPoint("descriptor_length", this->length, 2);
//     std::cout << std::string(2 * 4, ' ') << "descriptor_data: ";
//     for (auto ch : this->data) std::cout << ch;
//     std::cout << std::endl;
//     utils::printLine("/DESCRIPTOR/", 2, '-');
// }

std::uint8_t* Descriptors::Descriptor::getEndPointer() {
    std::uint8_t* end = this->end_pointer;
    this->end_pointer = nullptr;
    return end;
}