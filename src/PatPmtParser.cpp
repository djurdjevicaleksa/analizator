#include "../include/PatPmtParser.h"
#include <iostream>
#include <iomanip>
#include <cstring>




// Stream type IDs (subset)
#define STREAM_TYPE_MPEG1_VIDEO 0x01
#define STREAM_TYPE_MPEG2_VIDEO 0x02
#define STREAM_TYPE_MPEG1_AUDIO 0x03
#define STREAM_TYPE_MPEG2_AUDIO 0x04
#define STREAM_TYPE_PRIVATE_DATA 0x06
#define STREAM_TYPE_ADTS_AUDIO 0x0F
#define STREAM_TYPE_H264_VIDEO 0x1B
#define STREAM_TYPE_H265_VIDEO 0x24
#define STREAM_TYPE_AC3_AUDIO 0x81
#define STREAM_TYPE_DTS_AUDIO 0x82

// Descriptor tags
#define DESCRIPTOR_LANGUAGE 0x0A
#define DESCRIPTOR_SUBTITLING 0x59
#define DESCRIPTOR_TELETEXT 0x56
#define DESCRIPTOR_AC3 0x6A
#define DESCRIPTOR_ENHANCED_AC3 0x7A
#define DESCRIPTOR_SERVICE 0x48



const char* get_codec_name(uint8_t stream_type) {
    switch (stream_type) {
        case STREAM_TYPE_MPEG1_VIDEO: return "MPEG-1 Video";
        case STREAM_TYPE_MPEG2_VIDEO: return "MPEG-2 Video";
        case STREAM_TYPE_H264_VIDEO: return "H.264";
        case STREAM_TYPE_H265_VIDEO: return "H.265";
        case STREAM_TYPE_MPEG1_AUDIO: return "MPEG-1 Audio";
        case STREAM_TYPE_MPEG2_AUDIO: return "MPEG-2 Audio";
        case STREAM_TYPE_ADTS_AUDIO: return "ISO/IEC 13818-7 Audio with ADTS transport syntax";
        case STREAM_TYPE_AC3_AUDIO: return "AC-3";
        case STREAM_TYPE_DTS_AUDIO: return "DTS";
        case STREAM_TYPE_PRIVATE_DATA: return "Private Data";
        default: return "Unknown";
    }
}





static StreamType get_stream_type(uint8_t stream_type_id) {
    switch (stream_type_id) {
        case STREAM_TYPE_MPEG1_VIDEO:
        case STREAM_TYPE_MPEG2_VIDEO:
        case STREAM_TYPE_H264_VIDEO:
        case STREAM_TYPE_H265_VIDEO:
            return StreamType::VIDEO;
        case STREAM_TYPE_MPEG1_AUDIO:
        case STREAM_TYPE_MPEG2_AUDIO:
        case STREAM_TYPE_ADTS_AUDIO:
        case STREAM_TYPE_AC3_AUDIO:
        case STREAM_TYPE_DTS_AUDIO:
            return StreamType::AUDIO;
        case STREAM_TYPE_PRIVATE_DATA:
            return StreamType::DATA; // Could be subtitle or other data
        default:
            return StreamType::DATA;
    }
}



static void parse_descriptors(const uint8_t* data, size_t length, StreamInfo& stream_info) {
    size_t pos = 0;
    while (pos + 2 < length) {
        uint8_t tag = data[pos];
        uint8_t desc_length = data[pos + 1];

        std::cout << "  >> Descriptor tag: 0x" << std::hex << (int)tag 
          << ", length: " << std::dec << (int)desc_length << "\n";


        if (pos + 2 + desc_length > length) break; // Prevent overflow

        switch (tag) {
            case DESCRIPTOR_LANGUAGE: {
                if (desc_length >= 3) {
                    stream_info.language.assign(reinterpret_cast<const char*>(data + pos + 2), 3);
                    stream_info.language += '\0'; // Null-terminate
                }
                break;
            }
            case DESCRIPTOR_SUBTITLING:
                stream_info.type = StreamType::SUBTITLE;
                stream_info.codec = "Subtitle";  // ⬅️ dodaj ovo
                stream_info.description.assign("Subtitle");
                if (desc_length >= 3) {
                    stream_info.language.assign(reinterpret_cast<const char*>(data + pos + 2), 3);
                    stream_info.language += '\0'; // Null-terminate
                }
                break;
            case DESCRIPTOR_TELETEXT:
                stream_info.type = StreamType::SUBTITLE;
                stream_info.description.assign("Teletext");
                stream_info.codec = "Teletext";  // ⬅️ dodaj ovo
                break;
            case DESCRIPTOR_AC3:
                stream_info.codec = "AC-3";
                stream_info.type = StreamType::AUDIO;
                stream_info.description.assign("Dolby AC-3");
                break;
            case DESCRIPTOR_ENHANCED_AC3:
                // Handle AC-3 specific descriptors
                stream_info.codec = "Enhanced AC-3";
                stream_info.type = StreamType::AUDIO;
                stream_info.description.assign("Enhanced AC-3");
                break;
            default:
                // Unknown descriptor, skip
                break;
        }

        pos += 2 + desc_length;
    }
    std::cout << "  >> Final descriptor result: " << stream_info.codec << ", type: " << (int)stream_info.type << "\n";
    std::cout << "  >> Raw descriptor data: ";
for (size_t i = 0; i < length; ++i)
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
std::cout << std::dec << "\n";

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

    int section_length = ((section[1] & 0x0F) << 8) | section[2];
    int section_end = 3 + section_length;  // 3 bajta zaglavlja pre section_length


    for (auto& prog : programs) {
        if (prog.pmt_pid != pid) continue;

        uint16_t pil = ((section[10] & 0x0F) << 8) | section[11];
        int pos = 12 + pil;
        prog.streams.clear();

        while (pos + 5 <= section_end) { // pos + 5 <= section.size() - 4
            if (pos + 5 > section_end) {
                break;
            }

            uint8_t stype = section[pos];
            uint16_t epid = ((section[pos + 1] & 0x1F) << 8) | section[pos + 2];
            uint16_t es_info_len = ((section[pos + 3] & 0x0F) << 8) | section[pos + 4];

            StreamInfo sinfo;
            sinfo.pid = epid;
            sinfo.stream_type_id = stype;
            sinfo.type = get_stream_type(stype);
            sinfo.codec = get_codec_name(stype);
            sinfo.description = "";
            sinfo.language = "";

            std::cout << "[PMT] stream_type_id = 0x" << std::hex << (int)stype
          << ", PID = " << std::dec << epid
          << ", codec = " << get_codec_name(stype)
          << ", type = " << (int)get_stream_type(stype) << "\n";

            
            std::cout << "  > Descriptor length = " << es_info_len << "\n";


            if (es_info_len > 0 && pos + 5 + es_info_len <= section.size()) {

                parse_descriptors(section.data() + pos + 5, es_info_len, sinfo);
            } 

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

void TsPaketParser::printInfo(const std::vector<ProgramInfo>& programs) {
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

void TsPaketParser::print(const std::vector<ProgramInfo>& program_infos, size_t& index) const{
    if (index > program_infos.size()) return;
    ProgramInfo& prog = const_cast<std::vector<ProgramInfo>&>(program_infos)[index];

    std::cout << "Program " << prog.program_number << " (PMT PID: 0x" 
    << std::hex << prog.pmt_pid << std::dec << ")\n";
    for (const auto& stream : prog.streams) {
        std::cout << "  - PID " << stream.pid 
        << ", Type: " << stream.codec << "\n";
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

void TsPaketParser::printPAT() const {
    if (!pat_found || programs.empty()) {
        std::cout << "[PAT] PAT tabela nije pronađena ili je prazna.\n";
        return;
    }

    std::cout << "=== Sadržaj PAT tabele ===\n";
    for (const auto& prog : programs) {
        std::cout << "  - Program broj: " << prog.program_number 
                  << ", PMT PID: 0x" << std::hex << std::setw(4) << std::setfill('0') 
                  << prog.pmt_pid << std::dec << "\n";
    }
}




