#include <iostream>
#include <iomanip>

#include "SDTParser.h"
#include "Utils.h"

void ServiceDescriptionSection::PacketHeader::print() const {
    utils::printLine("SDT TABLE HEADER");
    utils::printHex("table_id", table_id);
    utils::printHex("section_syntax_indicator", section_syntax_indicator);
    utils::printHex("section_length", section_length);
    utils::printHex("transport_stream_id", transport_stream_id);
    utils::printHex("version_number", version_number);
    utils::printHex("current_next_indicator", current_next_indicator);
    utils::printHex("section_number", section_number);
    utils::printHex("last_section_number", last_section_number);
    utils::printHex("original_network_id", original_network_id);
    utils::printLine("/SDT TABLE HEADER/", 1, '=');
}

void ServiceDescriptionSection::Service::print() const {
    utils::printLine("SERVICE", 2, '-');
    utils::printHex("service_id", service_id, 2);
    utils::printHex("eit_schedule_flag", eit_schedule_flag, 2);
    utils::printHex("eit_present_following_flag", eit_present_following_flag, 2);
    utils::printHex("running_status", running_status, 2);
    utils::printHex("free_ca_mode", free_ca_mode, 2);
    utils::printHex("descriptors_loop_length", descriptors_loop_length, 2);

    for (const auto& desc : descriptors) {
        desc.print();
    }

    utils::printLine("/SERVICE/", 2, '-');
}

void ServiceDescriptionSection::print() const {
    utils::printLine("SDT TABLE", 0, '=');

    header.print();

    utils::printLine("SERVICES", 1);
    for (const auto& service : services) {
        service.print();
    }
    utils::printLine("/SERVICES/", 1);

    utils::printHex("CRC", crc);
    utils::printLine("/SDT TABLE/", 0, '=');
}
