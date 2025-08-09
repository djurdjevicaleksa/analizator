#ifndef _TSPARSER_H
#define _TSPARSER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>

#include "TSPacket.h"

namespace TS {
    std::uint8_t getPacketSize(const char* input_file_name);
    void parseTransportStream(const char* input_file_name, std::vector<TSPacket>& out);
    std::string getNameByPID(std::uint16_t pid, const std::unordered_map<std::uint16_t, std::string>& user_defined_pid_names);
    void printPacketGroups(
        const std::unordered_map<std::uint16_t, std::vector<TSPacket*>>& grouped_packets,
        const std::unordered_map<std::uint16_t, std::string>& user_defined_pid_names
    );
} // namespace TS

#endif
