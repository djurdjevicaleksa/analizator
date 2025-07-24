#ifndef _SDTPARSER_H
#define _SDTPARSER_H

#include <vector>

#include "TSPacket.h"
#include "SDT.h"

namespace SDT {
    const uint32_t PID = 0x0011; 
    std::vector<ServiceDescriptionSection> parse(std::vector<TSPacket>&);
}

#endif // _SDTPARSER_H