#include "PidMapper.h"
#include <iostream>
#include <sstream>
#include <iomanip>

std::string PidMapper::streamCategory(uint8_t st)
{
    switch (st) {
        case 0x01: case 0x02: case 0x1B: case 0x24: return "video";
        case 0x03: case 0x04: case 0x0F:            return "audio";
        case 0x06:                                  return "subtitles";
        case 0x05:                                  return "private";
        case 0x0D:                                  return "metadata";
        case 0x80:                                  return "ecm";
        case 0x81:                                  return "emm";
        default:                                    return "unknown";
    }
}
bool PidMapper::isAv(uint8_t st)
{
    return st==0x01||st==0x02||st==0x1B||st==0x24||
           st==0x03||st==0x04||st==0x0F;
}

void PidMapper::printByService(const TsPaketParser& patParser)
{
    const auto& programs = patParser.getPrograms();

    std::cout << "\n=== PID map by TV service ===\n";
    for (const auto& prg : programs) {
        std::cout << "Program " << prg.program_number
                  << "  (PMT PID: 0x"
                  << std::hex << prg.pmt_pid << std::dec << ")\n";

        for (const auto& st : prg.streams) {
            std::string pidDisp;
            if (isAv(st.stream_type_id))
                pidDisp = std::to_string(st.pid);      // decimal for AV
            else {
                std::ostringstream tmp;
                tmp << "0x" << std::hex << st.pid;
                pidDisp = tmp.str();
            }
            std::cout << "  PID: " << pidDisp
                      << "  │  Codec: "     << st.codec
                      << "  │  Category: "  << streamCategory(st.stream_type_id)
                      << '\n';
        }
        std::cout << '\n';
    }
}
