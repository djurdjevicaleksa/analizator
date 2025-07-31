#ifndef _ANALIZATOR_H
#define _ANALIZATOR_H

#include <vector>
#include <unordered_map>
#include <string>

#include "TSPacket.h"
#include "NIT.h"
#include "SDT.h"
#include "PatPmtParser.h"
#include "TSParser.h"

class Analizator {
public:
    std::vector<TSPacket> ts_packets;
    std::unordered_map<uint16_t, std::vector<TSPacket>> grouped_ts_packets;
    std::vector<NetworkInformationSection> nit_tables;
    std::vector<ServiceDescriptionSection> sdt_tables;
    std::vector<ProgramInfo> program_infos;
    std::unordered_map<uint16_t, std::string> custom_pid_names;

private:
    void getNITs();
    void getSDTs();
    void getProgramInfos();
    TsPaketParser pat_pmt_parser; 

public:
    Analizator() = default;
    Analizator(const char*);
    ~Analizator() = default;

    inline void printTSPacket(size_t index) const {
        if (index < this->ts_packets.size()) (this->ts_packets)[index].print();
    }

    inline void printGroupedPackets() const {
        TSParser parser;
        parser.printGroupedPackets(this->grouped_ts_packets, this->custom_pid_names);
    }

    inline void printPAT() const {
        this->pat_pmt_parser.printPAT();
    }

    inline void printPMT(size_t index) const {
        this->pat_pmt_parser.printPMT(this->program_infos, index);
    }

    inline void printNIT(size_t index) const {
        if (index < this->nit_tables.size()) this->nit_tables[index].print();
    };

    inline void printSDT(size_t index) const {
        if (index < this->sdt_tables.size()) this->sdt_tables[index].print();
    };

    void trackPMTChanges();
};

#endif // _ANALIZATOR_H