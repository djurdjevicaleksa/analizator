#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <cassert>

#include "src/Application/Analizator.h"
#include "src/Application/Parsers/TSParser.h"
#include "src/Application/Parsers/NITParser.h"
#include "src/Application/Parsers/SDTParser.h"
#include "src/Application/Parsers/PatPmtParser.h"


Analizator::Analizator(const char* input_ts_file_path) {

    TS::parseTransportStream(input_ts_file_path, this->ts_packets);
    this->groupPacketsByPID();
    NIT::parse(this->ts_by_pid, this->nit_tables);
    SDT::parse(this->ts_by_pid, this->sdt_tables);


    // this->getProgramInfos();
}

void Analizator::groupPacketsByPID() {

    for (std::size_t i = 0; i < this->ts_packets.size(); i++) 
        this->ts_by_pid[this->ts_packets[i].header.pid].emplace_back(&this->ts_packets[i]);
}

// void Analizator::getProgramInfos() {
//     this->pat_pmt_parser.parseFromGroupedPackets(this->grouped_ts_packets);
//     this->program_infos = std::move(pat_pmt_parser.getPrograms());

//     for (const auto& program : pat_pmt_parser.getPrograms()) {
//         this->custom_pid_names[program.pmt_pid] = "PMT (Program Map Table)";

//         for (const auto& stream : program.streams) {
//             std::string type_str;
//             switch (stream.type) {
//                 case StreamType::VIDEO:    type_str = "Video Stream"; break;
//                 case StreamType::AUDIO:    type_str = "Audio Stream"; break;
//                 case StreamType::SUBTITLE: type_str = "Subtitle Stream"; break;
//                 case StreamType::DATA:     type_str = "Data Stream"; break;
//                 default:                   type_str = "Unknown Stream"; break;
//             }

//             this->custom_pid_names[stream.pid] = type_str + " (" + stream.codec + ")";
//         }
//     }
// }