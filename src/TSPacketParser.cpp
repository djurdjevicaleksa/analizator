#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <cstring>

#include "../include/TSPacket.h"

constexpr int TS_PACKET_SIZE_STANDARD = 188;
constexpr int TS_PACKET_SIZE_EXTENDED = 192;
constexpr int TS_PACKET_SIZE_DVB = 204;

constexpr uint8_t SYNC_BYTE = 0x47;

TSPacket parseTSPacket(const uint8_t* packet, int packet_size, int index) {
    if (packet[0] != SYNC_BYTE) {
        std::cerr << "Packet " << index << " does not start with sync byte!\n";
        exit(1); 
    }


    /* Potencijalni fix jer je adaptation field control uvek 0 */
    TSPacket ts_packet;
    ts_packet.header.bf.sync_byte = SYNC_BYTE;
    ts_packet.header.bf.transport_error_indicator = packet[1] & (0b1 << 7);
    ts_packet.header.bf.payload_unit_start_indicator = packet[1] & (0b1 << 6);
    ts_packet.header.bf.transport_priority = packet[1] & (0b1 << 5);
    ts_packet.header.bf.pid = ((packet[1] & 0b11111) << 8) | packet[2];
    ts_packet.header.bf.transport_scrambling_control = packet[3] & (0b11 << 6);
    ts_packet.header.bf.adaptation_field_control = packet[3] & (0b11 << 4);   //ako je 0b10 - nema payload, ako je 0b11 - ima payload
    ts_packet.header.bf.continuity_counter = packet[3] & 0b1111;

    ts_packet.payload = new uint8_t[packet_size - 4];
    memcpy(ts_packet.payload, &packet[4], packet_size - 4);

    ts_packet.print();
    return ts_packet;
}

int getPacketSize(const char* ts_file) {

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
    std::cout << "Packet size: " << packet_size << std::endl;

    tsFile.close();
    return packet_size;
}

std::vector<TSPacket> parse(const char* ts_file) {

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

int main(int argc, char** argv) {
    parse(argv[1]);

    return 0;
}