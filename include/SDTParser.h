#ifndef _SDTPARSER_H
#define _SDTPARSER_H

#include <vector>

#include "TSPacket.h"
#include "SDT.h"

namespace SDT {
    std::vector<ServiceDescriptionSection> parse(std::vector<TSPacket>&);
}

#endif // _SDTPARSER_H