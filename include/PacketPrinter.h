#pragma once
#include "../include/TSPacket.h"
#include <cstddef>
#include <cstdint>

class PacketPrinter
{
public:
    /* Ispiše ceo TS-paket (188 B) u HEX-u + zaglavlje */
    static void printPacketHex(const TSPacket& pkt,
                               const uint8_t* raw,
                               std::size_t    size,
                               int            index);
};
