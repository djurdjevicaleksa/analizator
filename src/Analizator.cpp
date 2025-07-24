#include <cstdint>

#include "Analizator.h"
#include "TSParser.h"
#include "NITParser.h"
#include "SDTParser.h"

Analizator::Analizator(const char* input_ts_file_path) {
    TSParser ts_parser;
    this->ts_packets = std::move(ts_parser.parseTransportStream(input_ts_file_path));
    this->grouped_ts_packets = std::move(ts_parser.groupPacketsByPID(this->ts_packets));

    this->getNITs();
    this->getSDTs();
    //...
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