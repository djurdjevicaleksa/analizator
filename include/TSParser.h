#ifndef _TSPARSER_H
#define _TSPARSER_H

#include <vector>
#include <unordered_map>

#include "../include/TSPacket.h"

class TSParser {
    public:
        TSParser() = default;
        std::vector<TSPacket> parseTransportStream(const char* ts_file);
        int getPacketSize(const char* ts_file);
        TSPacket parseTSPacket(const uint8_t* packet, int packet_size, int index);
        std::unordered_map<uint16_t, std::vector<TSPacket>> groupPacketsByPID(const std::vector<TSPacket>& ts_packets);
        void printGroupedPackets(std::unordered_map<uint16_t, std::vector<TSPacket>> pid_groups);
};

#endif