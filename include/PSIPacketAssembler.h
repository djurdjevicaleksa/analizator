#ifndef _PSI_PACKET_ASSEMBLER_H
#define _PSI_PACKET_ASSEMBLER_H

#include <vector>
#include <cstdint>

#include "TSPacket.h"

class PSIPacketAssembler {
public:
    std::vector<std::vector<uint8_t>> process(const std::vector<TSPacket>& packets);
private:
    std::vector<uint8_t> buffer;
    size_t expected_section_length = 0;
    bool collecting = false;
    int last_cc = -1;

    void reset();
};

#endif // _PSI_PACKET_ASSEMBLER_H