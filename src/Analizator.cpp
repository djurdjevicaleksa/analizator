#include <cstdint>
#include <algorithm>

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
