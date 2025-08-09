#ifndef _PSI_PACKET_ASSEMBLER_H
#define _PSI_PACKET_ASSEMBLER_H

#include <vector>
#include <cstdint>

#include "TSPacket.h"

namespace PSI {
    
    void process(const std::vector<TSPacket*>& grouped_psi_packets, std::vector<std::vector<std::uint8_t>>& out_completed_sections);

} // namespace PSI


#endif // _PSI_PACKET_ASSEMBLER_H