#include "../include/PatPmtParser.h"
#include <iostream>
#include <iomanip>

const char* get_codec_name(uint8_t stream_type) {
    switch (stream_type) {
        case 0x01: return "MPEG-1 Video";
        case 0x02: return "MPEG-2 Video";
        case 0x1B: return "H.264";
        case 0x24: return "H.265";
        case 0x03: return "MPEG-1 Audio";
        case 0x04: return "MPEG-2 Audio";
        case 0x0F: return "AAC (ADTS)";
        case 0x81: return "AC-3";
        case 0x82: return "DTS";
        case 0x06: return "Private Data";
        default:   return "Unknown";
    }
}

StreamType get_stream_type(uint8_t stream_type_id) {
    switch (stream_type_id) {
        case 0x01: case 0x02: case 0x1B: case 0x24:
            return StreamType::VIDEO;
        case 0x03: case 0x04: case 0x0F: case 0x81: case 0x82:
            return StreamType::AUDIO;
        case 0x06:
            return StreamType::DATA;
        default:
            return StreamType::DATA;
    }
}

TsPaketParser::TsPaketParser() : packet_count(0), pat_found(false), pmt_found(false) {
    pat_section.buffer.resize(MAX_SECTION_SIZE);
}

bool TsPaketParser::parsePAT(const std::vector<uint8_t>& section) {
    if (section.size() < 8 || section[0] != 0x00) return false;

    int sl = ((section[1] & 0x0F) << 8) | section[2];
    int pos = 8;
    int max = sl - 9;

    programs.clear();

    while (pos < max + 8) {
        if (pos + 4 > section.size()) break;

        uint16_t prog = (section[pos] << 8) | section[pos + 1];
        uint16_t pmt_pid = ((section[pos + 2] & 0x1F) << 8) | section[pos + 3];

        if (prog != 0) {
            programs.push_back({prog, pmt_pid});
        }

        pos += 4;
    }

    pat_found = !programs.empty();
    return pat_found;
}

bool TsPaketParser::parsePMT(uint16_t pid, const std::vector<uint8_t>& section) {
    if (section.size() < 12 || section[0] != 0x02) return false;

    for (auto& prog : programs) {
        if (prog.pmt_pid != pid) continue;

        uint16_t pil = ((section[10] & 0x0F) << 8) | section[11];
        int pos = 12 + pil;
        prog.streams.clear();

        while (pos + 5 <= section.size() - 4) {
            uint8_t stype = section[pos];
            uint16_t epid = ((section[pos + 1] & 0x1F) << 8) | section[pos + 2];
            uint16_t es_info_len = ((section[pos + 3] & 0x0F) << 8) | section[pos + 4];

            StreamInfo sinfo;
            sinfo.pid = epid;
            sinfo.stream_type_id = stype;
            sinfo.type = get_stream_type(stype);
            sinfo.codec = get_codec_name(stype);
            prog.streams.push_back(sinfo);

            pos += 5 + es_info_len;
        }

        pmt_found = true;
        return true;
    }

    return false;
}

bool TsPaketParser::parsePacket(const uint8_t* packet) {
    return false; // not used in grouped packets context
}

bool TsPaketParser::parseFromGroupedPackets(const std::unordered_map<uint16_t, std::vector<TSPacket>>& grouped_packets) {
    auto pat_it = grouped_packets.find(0x0000);
    if (pat_it == grouped_packets.end()) return false;

    for (const auto& packet : pat_it->second) {
        if (!packet.payload || packet.payload_length == 0) continue;
        if (!packet.header.bf.payload_unit_start_indicator) continue;

        uint8_t pointer_field = packet.payload[0];
        if (pointer_field >= packet.payload_length) continue;

        const uint8_t* section_start = packet.payload + 1 + pointer_field;
        int section_len = packet.payload_length - 1 - pointer_field;

        std::vector<uint8_t> section(section_start, section_start + section_len);

        if (parsePAT(section)) break;
    }

    for (const auto& prog : programs) {
        auto it = grouped_packets.find(prog.pmt_pid);
        if (it == grouped_packets.end()) continue;

        for (const auto& packet : it->second) {
            if (!packet.payload || packet.payload_length == 0) continue;
            if (!packet.header.bf.payload_unit_start_indicator) continue;

            uint8_t pointer_field = packet.payload[0];
            if (pointer_field >= packet.payload_length) continue;

            const uint8_t* section_start = packet.payload + 1 + pointer_field;
            int section_len = packet.payload_length - 1 - pointer_field;

            std::vector<uint8_t> section(section_start, section_start + section_len);

            if (parsePMT(prog.pmt_pid, section)) break;
        }
    }

    return isComplete();
}

bool TsPaketParser::isComplete() const {
    return pat_found && pmt_found && !programs.empty();
}

void TsPaketParser::printInfo() const {
    std::cout << "=== PMT PID-ovi pronađeni u PAT tabeli ===\n";
    for (const auto& prog : programs) {
        std::cout << "  - PMT PID: 0x" << std::hex << prog.pmt_pid << std::dec
                  << " (Program No: " << prog.program_number << ")\n";
    }

    std::cout << "\n=== Detalji o programima ===\n";
    for (const auto& prog : programs) {
        std::cout << "Program " << prog.program_number << " (PMT PID: 0x" 
                  << std::hex << prog.pmt_pid << std::dec << ")\n";
        for (const auto& stream : prog.streams) {
            std::cout << "  - PID " << stream.pid 
                      << ", Type: " << stream.codec << "\n";
        }
    }
}


void TsPaketParser::getProgress(int& with_streams, int& total) const {
    total = programs.size();
    with_streams = 0;
    for (const auto& p : programs) {
        if (!p.streams.empty()) with_streams++;
    }
}

const std::vector<ProgramInfo>& TsPaketParser::getPrograms() const {
    return programs;
}
