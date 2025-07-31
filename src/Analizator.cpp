#include <cstdint>
#include <algorithm>
#include <cassert>

#include "Analizator.h"
#include "TSParser.h"
#include "NITParser.h"
#include "SDTParser.h"

#include "PatPmtParser.h"

Analizator::Analizator(const char* input_ts_file_path) {
    TSParser ts_parser;
    this->ts_packets = std::move(ts_parser.parseTransportStream(input_ts_file_path));
    this->grouped_ts_packets = std::move(ts_parser.groupPacketsByPID(this->ts_packets));

    this->getNITs();
    this->getSDTs();
    this->getProgramInfos();
}

void Analizator::getProgramInfos() {
    this->pat_pmt_parser.parseFromGroupedPackets(this->grouped_ts_packets);
    this->program_infos = std::move(pat_pmt_parser.getPrograms());

    for (const auto& program : pat_pmt_parser.getPrograms()) {
        this->custom_pid_names[program.pmt_pid] = "PMT (Program Map Table)";

        for (const auto& stream : program.streams) {
            std::string type_str;
            switch (stream.type) {
                case StreamType::VIDEO:    type_str = "Video Stream"; break;
                case StreamType::AUDIO:    type_str = "Audio Stream"; break;
                case StreamType::SUBTITLE: type_str = "Subtitle Stream"; break;
                case StreamType::DATA:     type_str = "Data Stream"; break;
                default:                   type_str = "Unknown Stream"; break;
            }

            this->custom_pid_names[stream.pid] = type_str + " (" + stream.codec + ")";
        }
    }
}

void Analizator::getNITs() {
    
    try {
        this->nit_tables = std::move(NIT::parse(this->grouped_ts_packets.at(NIT::PID)));
    }
    catch (std::out_of_range& e) {
        std::cerr << "[ANALIZATOR] Encountered an error while trying to parse NITs: " << e.what() << std::endl;
    }
}

void Analizator::getSDTs() {
    
    try {
        this->sdt_tables = std::move(SDT::parse(this->grouped_ts_packets.at(SDT::PID)));
    }
    catch (std::out_of_range& e) {
        std::cerr << "[ANALIZATOR] Encountered an error while trying to parse SDTs: " << e.what() << std::endl;
    }
}

void Analizator::trackPMTChanges() {

    if (this->program_infos.size() < 0) {
        std::cout << "[PMT TRACK] No changes detected." << std::endl;
    }

    for (size_t i = 1; i < this->program_infos.size(); i++) {
        if(this->program_infos[i - 1].version != this->program_infos[i].version) {

            // Nadji TS pakete koji nose PCR info
            uint16_t& target_ts_packet_pid = this->program_infos[i].pcr_pid;
            std::vector<TSPacket> ts_packets = this->grouped_ts_packets.at(target_ts_packet_pid);

            // Izbaci pakete koji nemaju adaption field
            ts_packets.erase(
                std::remove_if(ts_packets.begin(), ts_packets.end(), [](TSPacket& obj) {return obj.adaptation_field == nullptr;}), ts_packets.end()
            );

            ts_packets.erase(
                std::remove_if(ts_packets.begin(), ts_packets.end(), [](TSPacket& obj)
                    {
                        uint8_t* adaption_field_start = obj.adaptation_field;
                        uint8_t* adaption_field_end = adaption_field_start + obj.adaptation_field_length;

                        if (adaption_field_start + 1 > adaption_field_end) return true;
                        if (!((obj.adaptation_field[1]  >> 4) & 0b1)) return true;
                        return false;
                    }
                ),
                ts_packets.end()
            );

            for (auto it = ts_packets.begin(); it < ts_packets.end(); it++) {

                TSPacket& current_packet = *it;

                uint8_t* adaption_field_start = current_packet.adaptation_field;
                uint8_t* adaption_field_end = adaption_field_start + current_packet.adaptation_field_length;

                uint8_t* pcr_field = &adaption_field_start[2];
                
                assert(pcr_field < adaption_field_end);
                assert(pcr_field + 6 <= adaption_field_end);

                
                uint64_t pcr_base = ((uint64_t)pcr_field[0] << 25) |
                    ((uint64_t)pcr_field[1] << 17) |
                    ((uint64_t)pcr_field[2] << 9)  |
                    ((uint64_t)pcr_field[3] << 1)  |
                    ((uint64_t)pcr_field[4] >> 7);

                uint16_t pcr_extension = ((pcr_field[4] & 0b1) << 8) | pcr_field[5];

                uint64_t pcr = pcr_base * 300 + pcr_extension;
                double pcr_time_seconds = (double)pcr / 27e6;

                std::cout << "TIME: " << pcr_time_seconds << std::endl;
            }

        }
    }
}

