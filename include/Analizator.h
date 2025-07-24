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
    std::vector<TSPacket> ts_packets;
    std::unordered_map<uint16_t, std::vector<TSPacket>> grouped_ts_packets;
    std::vector<NetworkInformationSection> nit_tables;
    std::vector<ServiceDescriptionSection> sdt_tables;
    std::vector<ProgramInfo> program_infos;
    std::unordered_map<uint16_t, std::string> custom_pid_names;

    void getNITs();
    void getSDTs();
    void getProgramInfos();

public:
    Analizator(const char*);
    Analizator() = delete;
    ~Analizator() = default;

    inline void printNITs() const {
        for (auto& nit: this->nit_tables) nit.print();
    }

    inline void printNIT(size_t index) const {
        if (index < this->nit_tables.size()) this->nit_tables[index].print();
    };

    inline void printSDTs() const {
        for (auto& sdt: this->sdt_tables) sdt.print();
    }

    inline void printSDT(size_t index) const {
        if (index < this->sdt_tables.size()) this->sdt_tables[index].print();
    };

    inline void printProgramInfos() const {
        TsPaketParser::printInfo(this->program_infos);
    }

    inline void printGroupedPackets() const {
        TSParser parser;
        parser.printGroupedPackets(this->grouped_ts_packets, this->custom_pid_names);
    }
};

#endif // _ANALIZATOR_H