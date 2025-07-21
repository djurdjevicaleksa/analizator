#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <cstring>

#include "../include/TSPacket.h"

constexpr int TS_PACKET_SIZE = 188;
constexpr int TS_PAYLOAD_SIZE = TS_PACKET_SIZE - 4;
constexpr uint8_t SYNC_BYTE = 0x47;

TSPacket parseTSPacket(const uint8_t* packet, int index) {
    if (packet[0] != SYNC_BYTE) {
        std::cerr << "Packet " << index << " does not start with sync byte!\n";
        exit(1); 
    }

    TSPacket ts_packet;
    ts_packet.header.bf.sync_byte = SYNC_BYTE;
    ts_packet.header.bf.transport_error_indicator = packet[1] & (0b1 << 7);
    ts_packet.header.bf.payload_unit_start_indicator = packet[1] & (0b1 << 6);
    ts_packet.header.bf.transport_priority = packet[1] & (0b1 << 5);
    ts_packet.header.bf.pid = ((packet[1] & 0b11111) << 8) | packet[2];
    ts_packet.header.bf.transport_scrambling_control = packet[3] & (0b11 << 6);
    ts_packet.header.bf.adaptation_field_control = packet[3] & (0b11 << 4);   //ako je 0b10 - nema payload, ako je 0b11 - ima payload
    ts_packet.header.bf.continuity_counter = packet[3] & 0b1111;

    ts_packet.payload = new uint8_t[TS_PAYLOAD_SIZE];
    memcpy(ts_packet.payload, &packet[4], TS_PAYLOAD_SIZE);

    ts_packet.print();
    return ts_packet;
}

std::vector<TSPacket> parse(const char* ts_file) {
    std::vector<TSPacket> ts_packets;

    std::ifstream tsFile(ts_file, std::ios::binary);
    if (!tsFile.is_open()) {
        std::cerr << "Failed to open TS file.\n";
        exit(1);
    }

    uint8_t packet[TS_PACKET_SIZE];
    int packetCount = 0;
    
    while (tsFile.read(reinterpret_cast<char*>(packet), TS_PACKET_SIZE)) {
        ts_packets.push_back(std::move(parseTSPacket(packet, packetCount++)));
    }

    tsFile.close();
    return ts_packets;
}

int main(int argc, char** argv) {
    parse(argv[1]);

    return 0;
}