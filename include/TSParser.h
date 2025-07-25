#ifndef _TSPARSER_H
#define _TSPARSER_H

#include <vector>
#include <unordered_map>

#include "TSPacket.h"

class TSParser {
    public:
        TSParser() = default;
        std::vector<TSPacket> parseTransportStream(const char* ts_file);
        
        int getPacketSize(const char* ts_file);
        TSPacket parseTSPacket(const uint8_t* packet, int packet_size);

        std::unordered_map<uint16_t, std::vector<TSPacket>> groupPacketsByPID(const std::vector<TSPacket>& ts_packets);
        void printGroupedPackets(const std::unordered_map<uint16_t, std::vector<TSPacket>>& pid_groups, const std::unordered_map<uint16_t, std::string>& custom_pid_names);

        const char* getPIDName(uint16_t pid, const std::unordered_map<uint16_t, std::string>& custom_pid_names);
};

#endif
