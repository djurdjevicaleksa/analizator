#ifndef _ANALIZATOR_H
#define _ANALIZATOR_H

#include <vector>
#include <unordered_map>
#include <string>

#include "TSPacket.h"
#include "NIT.h"
#include "SDT.h"
#include "src/Application/Parsers/PatPmtParser.h"
#include "src/Application/Parsers/TSParser.h"

class Analizator {
public:
    std::vector<TSPacket> ts_packets;
    std::unordered_map<std::uint16_t, std::vector<TSPacket*>> ts_by_pid;
    std::vector<NetworkInformationSection> nit_tables;
    std::vector<ServiceDescriptionSection> sdt_tables;

    
    std::vector<ProgramInfo> program_infos;
    std::unordered_map<uint16_t, std::string> custom_pid_names;

private:
    // void getProgramInfos();
    void groupPacketsByPID();
    TsPaketParser pat_pmt_parser; 

public:
    Analizator(const char*);
    
    Analizator() = default;
    
    Analizator(const Analizator&) = delete;
    Analizator& operator=(const Analizator&) = delete;
    
    Analizator(Analizator&&) = default;
    Analizator& operator=(Analizator&&) = default;
    ~Analizator() = default;

    inline void printTSPacket(size_t index) const {
        if (index < this->ts_packets.size()) (this->ts_packets)[index].print();
    }

    inline void printGroupedPackets() const {
        TS::printPacketGroups(this->ts_by_pid, this->custom_pid_names);
    }

    inline void printPAT() const {
        this->pat_pmt_parser.printPAT();
    }

    inline void printPMT(size_t index) const {
        this->pat_pmt_parser.printPMT(this->program_infos, index);
    }

    inline void printNIT(size_t index) const {
        if (index < this->nit_tables.size()) this->nit_tables[index].print(1);
    };

    inline void printSDT(size_t index) const {
        if (index < this->sdt_tables.size()) this->sdt_tables[index].print(1);
    };
};

#endif // _ANALIZATOR_H