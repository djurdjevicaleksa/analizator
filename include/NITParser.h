#ifndef _NITPARSER_H
#define _NITPARSER_H

#include <vector>
#include "NIT.h"
#include "TSPacket.h"

namespace NIT {
    std::vector<NetworkInformationSection> parse(std::vector<TSPacket>&);
}

#endif // _NITPARSER_H