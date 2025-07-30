#include <cstddef>
#include <cstdint>

#include "CableDeliverySystemDescriptor.h"
#include "Utils.h"

Descriptors::CableDeliverySystemDescriptor::CableDeliverySystemDescriptor(std::size_t length, const std::uint8_t* start)
    : Descriptors::Descriptor(length) {

    this->frequency = utils::readMSB32(start);
    this->fec_outer = start[5] & 0b1111; 
    this->modulation = start[6];
    this->symbol_rate = utils::readMSB32(&start[7]) >> 4;
    this->fec_inner = start[10] & 0b1111;
}

double Descriptors::CableDeliverySystemDescriptor::deduceFrequency() const {
    std::uint32_t value = 0;

    for (std::size_t i = 0; i < 8; i++) {
        std::uint8_t new_digit = (this->frequency >> (28 - i * 4)) & 0b1111;
        if (new_digit > 9) throw std::runtime_error("[CableDeliverySystemDescriptor] Failed to deduce the frequency in MHz: Invalid frequency bitfield.");

        value = value * 10 + new_digit;
    }

    return static_cast<double>(value) / 10'000.0;
}

std::string Descriptors::CableDeliverySystemDescriptor::deduceFECOuterScheme() const {
    switch(this->fec_outer) {
        case 0: return "Not defined";
        case 1: return "No outer FEC coding";
        case 2: return "RS(204/188)";
        default: return "Reserved for future use";
    }
}

std::string Descriptors::CableDeliverySystemDescriptor::deduceModulationScheme() const {
    switch(this->modulation) {
        case 0: return "Not defined";
        case 1: return "16-QAM";
        case 2: return "32-QAM";
        case 3: return "64-QAM";
        case 4: return "128-QAM";
        case 5: return "256-QAM";
        default: return "Reserved for future use";
    }
}

double Descriptors::CableDeliverySystemDescriptor::deduceSymbolRate() const {
    
    std::uint32_t value = 0;

    for (std::size_t i = 0; i < 7; i++) {
        std::uint8_t new_digit = (this->symbol_rate >> (24 - i * 4)) & 0b1111;
        if (new_digit > 9) throw std::runtime_error("[CableDeliverySystemDescriptor] Failed to deduce the symbol rate: Invalid symbol rate bitfield.");

        value = value * 10 + new_digit;
    }

    return static_cast<double>(value) / 10'000.0;
}

std::string Descriptors::CableDeliverySystemDescriptor::deduceFECInnerScheme() const {
    if (this->fec_inner > 15) throw std::runtime_error("[CableDeliverySystemDescriptor] Failed to deduce the FEC scheme: Invalid fec inner value.");
    switch(this->fec_inner) {
        case 0: return "Not defined";
        case 1: return "1/2 convolutional code rate";
        case 2: return "2/3 convolutional code rate";
        case 3: return "3/4 convolutional code rate";
        case 4: return "5/6 convolutional code rate";
        case 5: return "7/8 convolutional code rate";
        case 6: return "8/9 convolutional code rate";
        case 7: return "3/5 convolutional code rate";
        case 8: return "4/5 convolutional code rate";
        case 9: return "9/10 convolutional code rate";
        case 15: return "no convolutional coding";
        default: return "Reserved for future use";
    }
}