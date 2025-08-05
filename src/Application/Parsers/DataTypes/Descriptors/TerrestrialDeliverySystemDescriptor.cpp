#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "src/Application/Parsers/DataTypes/Descriptors/DescriptorFactory.h"
#include "src/Application/Parsers/DataTypes/Descriptors/TerrestrialDeliverySystemDescriptor.h"

#include "src/Application/Utilities/Utils.h"

namespace {
    const bool registered = Descriptors::DescriptorFactory::instance().registerFactory(
        Descriptors::TerrestrialDeliverySystemDescriptor::tag,
        &Descriptors::create<Descriptors::TerrestrialDeliverySystemDescriptor::tag>
    );
}

Descriptors::TerrestrialDeliverySystemDescriptor::TerrestrialDeliverySystemDescriptor(std::size_t len, const std::uint8_t* start)
    : Descriptor(len) {

    this->centre_frequency = utils::readMSB32(start);
    this->bandwidth = (start[4] >> 5) & 0b111;
    this->priority = (start[4] >> 4) & 0b1;
    this->time_slicing_indicator = (start[4] >> 3) & 0b1;
    this->mpe_fec_indicator = (start[4] >> 2) & 0b1;
    this->constellation = (start[5] >> 6) & 0b11;
    this->hierarchy_information = (start[5] >> 3) & 0b111;
    this->code_rate_hp_stream = start[5] & 0b111;
    this->code_rate_lp_stream = (start[6] >> 5) & 0b111;
    this->guard_interval = (start[6] >> 3) & 0b11;
    this->transmission_mode = (start[6] >> 1) & 0b11;
    this->other_frequency_flag = start[6] & 0b1;
}

std::string Descriptors::TerrestrialDeliverySystemDescriptor::deduceBandwidth() const {
    switch(this->bandwidth) {
        case 0: return "8 MHz";
        case 1: return "7 MHz";
        case 2: return "6 MHz";
        case 3: return "5 MHz";
        default: return "Reserved for future use";
    }
}

std::string Descriptors::TerrestrialDeliverySystemDescriptor::deduceConstellationPattern() const {
    switch(this->constellation) {
        case 0: return "QPSK";
        case 1: return "16-QAM";
        case 2: return "64-QAM";
        default: return "Reserved for future use";
    }
}

std::string Descriptors::TerrestrialDeliverySystemDescriptor::deduceHierarchyInformation() const {
    switch(this->hierarchy_information) {
        case 0: return "Non-hierarchical, native interleaver";
        case 1: return "Alpha=1, native interleaver";
        case 2: return "Alpha=2, native interleaver";
        case 3: return "Alpha=4, native interleaver";
        case 4: return "Non-hierarchical, in-depth interleaver";
        case 5: return "Alpha=1, in-depth interleaver";
        case 6: return "Alpha=2, in-depth interleaver";
        case 7: return "Alpha=4, in-depth interleaver";
        default: throw std::runtime_error("[TerrestrialDeliverySystemDescriptor] Failed to deduce the hierarchy information: Invalid hierarchy information bitfield value.");
    }
}

std::string Descriptors::TerrestrialDeliverySystemDescriptor::deduceCodeRate() const {
    if (this->hierarchy_information == 0 || this->hierarchy_information == 4) {
        // Non-hierarchical
        switch(this->code_rate_hp_stream) {
            case 0: return "Non-hierarchical, 1/2 modulation";
            case 1: return "Non-hierarchical, 2/3 modulation";
            case 2: return "Non-hierarchical, 3/4 modulation";
            case 3: return "Non-hierarchical, 5/6 modulation";
            case 4: return "Non-hierarchical, 7/8 modulation";
            default: return "Reserved for future use";
        }
    }
    else {
        // Hierarchical
        std::string ret = "Hierarchical, ";
        switch(this->code_rate_hp_stream) {
            case 0: ret += "High-priority: 1/2 modulation"; break;
            case 1: ret += "High-priority: 2/3 modulation"; break;
            case 2: ret += "High-priority: 3/4 modulation"; break;
            case 3: ret += "High-priority: 5/6 modulation"; break;
            case 4: ret += "High-priority: 7/8 modulation"; break;
            default: ret += "High-priority: Reserved for future use"; break;
        }

        ret += "  ";

        switch(this->code_rate_lp_stream) {
            case 0: ret += "Low-priority: 1/2 modulation"; break;
            case 1: ret += "Low-priority: 2/3 modulation"; break;
            case 2: ret += "Low-priority: 3/4 modulation"; break;
            case 3: ret += "Low-priority: 5/6 modulation"; break;
            case 4: ret += "Low-priority: 7/8 modulation"; break;
            default: ret += "Low-priority: Reserved for future use"; break;
        }

        return ret;
    }
}

std::string Descriptors::TerrestrialDeliverySystemDescriptor::deduceGuardInterval() const {
    switch(this->guard_interval) {
        case 0: return "1/32";
        case 1: return "1/16";
        case 2: return "1/8";
        case 3: return "1/4";
        default: throw std::runtime_error("[TerrestrialDeliverySystemDescriptor] Failed to deduce the guard interval: Invalid guard interval bitfield value.");
    }
}

std::string Descriptors::TerrestrialDeliverySystemDescriptor::deduceTransmissionMode() const {
    switch(this->transmission_mode) {
        case 0: return "2K mode";
        case 1: return "8K mode";
        case 2: return "4K mode";
        case 3: return "Reserved for future use";
        default: throw std::runtime_error("[TerrestrialDeliverySystemDescriptor] Failed to deduce the transmission mode: Invalid transmission mode bitfield value.");
    }
}

void Descriptors::TerrestrialDeliverySystemDescriptor::print(std::size_t indent_level) const {

    utils::printLine("Terrestrial delivery system descriptor", indent_level, '=');
    utils::printDataPoint("Centre frequency", this->centre_frequency, indent_level, std::to_string(this->deduceCentreFrequencyGHz()));
    utils::printDataPoint("Bandwidth", this->bandwidth, indent_level, this->deduceBandwidth());
    utils::printDataPoint("Priority", this->priority, indent_level, this->deducePriority());
    utils::printDataPoint("Time slicing indicator", this->time_slicing_indicator, indent_level);
    utils::printDataPoint("MPE FEC indicator", this->mpe_fec_indicator, indent_level);
    utils::printDataPoint("Bandwidth", this->bandwidth, indent_level, this->deduceBandwidth());
    utils::printDataPoint("Constellation", this->constellation, indent_level, this->deduceConstellationPattern());
    utils::printDataPoint("Hierarchy information", this->hierarchy_information, indent_level, this->deduceHierarchyInformation());
    utils::printDataPoint("Code rate - High priority stream", this->code_rate_hp_stream, indent_level, this->deduceCodeRate());
    utils::printDataPoint("Code rate - Low priority stream", this->code_rate_lp_stream, indent_level);
    utils::printDataPoint("Guard interval", this->guard_interval, indent_level, this->deduceGuardInterval());
    utils::printDataPoint("Transmission mode", this->transmission_mode, indent_level, this->deduceTransmissionMode());
    utils::printDataPoint("Other frequency flag", this->other_frequency_flag, indent_level);
    utils::printLine("Terrestrial delivery system descriptor", indent_level, '=');
}
