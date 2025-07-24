#ifndef _NITPARSER_H
#define _NITPARSER_H

#include <vector>
#include "NIT.h"
#include "TSPacket.h"

namespace NIT {
    const uint32_t PID = 0x0010;
    std::vector<NetworkInformationSection> parse(std::vector<TSPacket>&);
}

#endif // _NITPARSER_H