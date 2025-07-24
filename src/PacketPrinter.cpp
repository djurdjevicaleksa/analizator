#include "PacketPrinter.h"
#include <iostream>
#include <iomanip>

void PacketPrinter::printPacketHex(const TSPacket& pkt,
                                   const uint8_t*  raw,
                                   std::size_t     size,
                                   int             index)
{
    std::cout << "\nPacket " << index << " (" << size << " bytes):\n";
    for (std::size_t i = 0; i < size; ++i) {
        if (i % 16 == 0)
            std::cout << std::setw(4) << std::setfill('0')
                      << std::hex << i << ": ";
        std::cout << std::setw(2) << std::setfill('0')
                  << std::hex << static_cast<int>(raw[i]) << ' ';
        if ((i + 1) % 16 == 0) std::cout << '\n';
    }
    std::cout << std::dec;

    //––– TS-header semantika –––––––––––––––––––––––––––––
    std::cout << "\n  Sync Byte: 0x" << std::hex << (raw[0] & 0xFF) << '\n'
              << "  Transport Error Indicator: "
              << ((raw[1] & 0x80) >> 7) << '\n'
              << "  Payload Unit Start Indicator: "
              << ((raw[1] & 0x40) >> 6) << '\n'
              << "  Transport Priority: "
              << ((raw[1] & 0x20) >> 5) << '\n'
              << "  PID: 0x"
              << (((raw[1] & 0x1F) << 8) | raw[2]) << '\n'
              << "  Scrambling Control: "
              << ((raw[3] & 0xC0) >> 6) << '\n'
              << "  Adaptation Field Control: "
              << ((raw[3] & 0x30) >> 4) << '\n'
              << "  Continuity Counter: "
              << (raw[3] & 0x0F) << '\n'
              << "--------------------------------------------------\n"
              << std::dec;
}
