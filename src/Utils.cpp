#include <iostream>

#include "Utils.h"

void utils::printLine(const std::string& label, int indent, char border) {
    std::cout << std::string(indent * 4, ' ')
                << std::string(5, border) << " " << label << " "
                << std::string(5, border) << std::endl;
}
