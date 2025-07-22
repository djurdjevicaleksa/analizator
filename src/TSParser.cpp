#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <cstring>

#include "../include/TSParser.h" 

// Possible packet sizes
constexpr int TS_PACKET_SIZE_STANDARD = 188;
constexpr int TS_PACKET_SIZE_EXTENDED = 192;
constexpr int TS_PACKET_SIZE_DVB = 204;

constexpr uint8_t SYNC_BYTE = 0x47;

TSPacket TSParser::parseTSPacket(const uint8_t* packet, int packet_size, int index) 
{
    if (packet[0] != SYNC_BYTE) {
        std::cerr << "Packet " << index << " does not start with sync byte!\n";
        exit(1); 
    }

    TSPacket ts_packet;
    ts_packet.header.bf.transport_error_indicator = packet[1] & 0x80;
    ts_packet.header.bf.payload_unit_start_indicator = packet[1] & 0x40;
    ts_packet.header.bf.transport_priority = packet[1] & 0x20;
    ts_packet.header.bf.pid = ((packet[1] & 0x1F) << 8) | packet[2];
    ts_packet.header.bf.transport_scrambling_control = (packet[3] & 0xC0) >> 6;

    uint8_t adaptation_field_control = (packet[3] & 0x30) >> 4;
    ts_packet.header.bf.adaptation_field_control = adaptation_field_control;

    ts_packet.header.bf.continuity_counter = packet[3] & 0x0F;

    switch (adaptation_field_control) {
        case 1: { // samo payload
            ts_packet.payload_length = packet_size - 4;
            ts_packet.payload = new uint8_t[ts_packet.payload_length];
            memcpy(ts_packet.payload, &packet[4], ts_packet.payload_length);
            break;
        }
        case 2: { // samo adaptation field
            ts_packet.adaptation_field_length = packet[4] + 1;
            ts_packet.adaptation_field = new uint8_t[ts_packet.adaptation_field_length];
            memcpy(ts_packet.adaptation_field, &packet[4], ts_packet.adaptation_field_length);
            break;
        }
        case 3: { // oba
            ts_packet.adaptation_field_length = packet[4] + 1;
            ts_packet.adaptation_field = new uint8_t[ts_packet.adaptation_field_length];
            memcpy(ts_packet.adaptation_field, &packet[4], ts_packet.adaptation_field_length);
    
            size_t payload_offset = 4 + ts_packet.adaptation_field_length;
            ts_packet.payload_length = packet_size - payload_offset;
            ts_packet.payload = new uint8_t[ts_packet.payload_length];
            memcpy(ts_packet.payload, &packet[payload_offset], ts_packet.payload_length);
            break;
        }
        default:
            break;
    }

    return ts_packet;
}

int TSParser::getPacketSize(const char* ts_file) 
{
    std::ifstream tsFile(ts_file, std::ios::binary);
    if (!tsFile.is_open()) {
        std::cerr << "Failed to open TS file.\n";
        exit(1);
    }

    int packet_size;
    uint8_t buffer[300];

    tsFile.read(reinterpret_cast<char*>(buffer), 300);
    if (buffer[TS_PACKET_SIZE_STANDARD] == 0x47) {
        packet_size = TS_PACKET_SIZE_STANDARD;
    }
    else if (buffer[TS_PACKET_SIZE_EXTENDED] == 0x47) {
        packet_size = TS_PACKET_SIZE_EXTENDED;
    }
    else if (buffer[TS_PACKET_SIZE_DVB] == 0x47) {
        packet_size = TS_PACKET_SIZE_DVB;
    }
    else {
        exit(1);
    }

    tsFile.close();
    return packet_size;
}

std::vector<TSPacket> TSParser::parseTransportStream(const char* ts_file) 
{
    int packet_size = getPacketSize(ts_file);
    uint8_t packet[packet_size];

    std::vector<TSPacket> ts_packets;
    int packetCount = 0;

    std::ifstream tsFile(ts_file, std::ios::binary);
    if (!tsFile.is_open()) {
        std::cerr << "Failed to open TS file.\n";
        exit(1);
    }
    
    while (tsFile.read(reinterpret_cast<char*>(packet), packet_size)) {
        ts_packets.push_back(std::move(parseTSPacket(packet, packet_size, packetCount++)));
    }

    tsFile.close();
    return ts_packets;
}

std::unordered_map<uint16_t, std::vector<TSPacket>> TSParser::groupPacketsByPID(const std::vector<TSPacket>& ts_packets)
 {
    std::unordered_map<uint16_t, std::vector<TSPacket>> pidGroups;

    for (const auto& packet : ts_packets) {
        uint16_t pid = packet.header.bf.pid;
        pidGroups[pid].push_back(packet);
    }

    return pidGroups;
}

void TSParser::printGroupedPackets(std::unordered_map<uint16_t, std::vector<TSPacket>> pid_groups)
{
    for (const auto& [pid, packets] : pid_groups) {
        std::cout << "PID: 0x" << std::hex << pid 
                  << " (" << std::dec << packets.size() << " packets)\n";
    }
}
