#pragma once
#include "../include/PatPmtParser.h"
#include <string>

class PidMapper
{
public:
    /* Štampa PID-ove grupisane po TV-servisu,
       uz codec i kategoriju (video/audio/…) */
    static void printByService(const TsPaketParser& patParser);

private:
    static std::string streamCategory(uint8_t stream_type);
    static bool        isAv(uint8_t stream_type);
};
