#ifndef _SDTPARSER_H
#define _SDTPARSER_H

#include <vector>
#include <unordered_map>

#include "SDT.h"
#include "TSPacket.h"

namespace SDT {
    const uint32_t PID = 0x0011;
    void parse(const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_ts_packets, std::vector<ServiceDescriptionSection>& out);
}

#endif // _SDTPARSER_H