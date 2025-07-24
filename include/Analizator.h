#ifndef _ANALIZATOR_H
#define _ANALIZATOR_H

#include <vector>
#include <unordered_map>

#include "TSPacket.h"
#include "NIT.h"
#include "SDT.h"

class Analizator {
    std::vector<TSPacket> ts_packets;
    std::unordered_map<uint16_t, std::vector<TSPacket>> grouped_ts_packets;
    std::vector<NetworkInformationSection> nit_tables;
    std::vector<ServiceDescriptionSection> sdt_tables;

    void getNITs();
    void getSDTs();

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
};

#endif // _ANALIZATOR_H