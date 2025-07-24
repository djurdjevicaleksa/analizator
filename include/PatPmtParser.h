#ifndef PATPMTPARSER_H
#define PATPMTPARSER_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "../include/TSPacket.h"

constexpr int TS_PACKET_SIZE = 188;
constexpr int MAX_SECTION_SIZE = 4096;

// Stream types
enum class StreamType {
    VIDEO = 0,
    AUDIO = 1,
    SUBTITLE = 2,
    DATA = 3
};

// Stream information
struct StreamInfo {
    uint16_t pid = 0;
    StreamType type = StreamType::DATA;
    uint8_t stream_type_id = 0;
    std::string codec;
    std::string language;
    std::string description;
};

// Program information
struct ProgramInfo {
    uint16_t program_number = 0;
    uint16_t pmt_pid = 0;
    std::vector<StreamInfo> streams;
};

// Section reassembly buffer
struct SectionBuffer {
    std::vector<uint8_t> buffer;
    int length = 0;
    int expected_length = 0;
    bool in_progress = false;

    SectionBuffer() : buffer(MAX_SECTION_SIZE) {}
};

// PAT/PMT parser class
class TsPaketParser {
public:
    TsPaketParser();

    // Parse one TS packet
    bool parsePacket(const uint8_t* packet);

    // Check if PAT and all PMTs are found
    bool isComplete() const;

    // Get the number of programs and those with streams
    void getProgress(int& programs_with_streams, int& total_programs) const;

    // Print all parsed information
    static void printInfo(const std::vector<ProgramInfo>&);

    // Get parsed program list
    const std::vector<ProgramInfo>& getPrograms() const;

    bool parseFromGroupedPackets(const std::unordered_map<uint16_t, std::vector<TSPacket>>& grouped_packets);


    bool parsePAT(const std::vector<uint8_t>& section);
    bool parsePMT(uint16_t pid, const std::vector<uint8_t>& section);

    void print(const std::vector<ProgramInfo>&, size_t&) const;

private:
    std::vector<ProgramInfo> programs;
    SectionBuffer pat_section;
    int packet_count = 0;
    bool pat_found = false;
    bool pmt_found = false;
};

#endif // PATPMTPARSER_H