#ifndef _NITPARSER_H
#define _NITPARSER_H

#include <vector>
#include <unordered_map>

#include "NIT.h"
#include "TSPacket.h"

namespace NIT {
    const uint32_t PID = 0x0010;
    void parse(const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_ts_packets, std::vector<NetworkInformationSection>& out);
}

#endif // _NITPARSER_H