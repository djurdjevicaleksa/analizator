#include <cstddef>
#include <cstdint>

#include "src/Application/Parsers/DataTypes/Descriptors/SatelliteDeliverySystemDescriptor.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Registrar.h"
#include "src/Application/Utilities/Utils.h"

const std::uint8_t Descriptors::SatelliteDeliverySystemDescriptor::tag = 0x43;
static Registrar<Descriptors::SatelliteDeliverySystemDescriptor> satellite_delivery_system_descriptor_registrar{ Descriptors::SatelliteDeliverySystemDescriptor::tag };

Descriptors::SatelliteDeliverySystemDescriptor::SatelliteDeliverySystemDescriptor(std::size_t length, const std::uint8_t* start)
    : Descriptors::Descriptor(length) {
    
    this->frequency = utils::readMSB32(start);
    this->orbital_position = utils::readMSB16(start + 4);
    this->west_east_flag = (start[6] >> 8) & 0b1;
    this->polarization = (start[6] >> 5) & 0b11;
    this->roll_off = (start[6] >> 3) & 0b11;
    this->modulation_system = (start[6] >> 2) & 0b1;
    this->modulation_type = start[6] & 0b11;
    this->symbol_rate = utils::readMSB32(&start[7]) >> 4;
    this->fec_inner = start[10] & 0b1111;
}

void Descriptors::SatelliteDeliverySystemDescriptor::print(std::size_t indent_level) const {

    utils::printLine("Satellite delivery system descriptor", indent_level, '=');
    utils::printDataPoint("Frequency", this->frequency, indent_level, std::to_string(this->deduceFrequencyGHz()));
    utils::printDataPoint("Orbital position", this->orbital_position, indent_level, std::to_string(this->deduceOrbitalPositionInDegrees()));
    utils::printDataPoint("West/East flag", this->west_east_flag, indent_level, std::to_string(this->deduceWestEast()));
    utils::printDataPoint("Polarization", this->polarization, indent_level, this->deducePolarization());
    utils::printDataPoint("Roll-off", this->roll_off, indent_level, std::to_string(this->deduceRollOff()));
    utils::printDataPoint("Modulation system", this->modulation_system, indent_level, this->deduceModulationSystem());
    utils::printDataPoint("Modulation type", this->modulation_type, indent_level, this->deduceModulationType());
    utils::printDataPoint("Symbol rate", this->symbol_rate, indent_level, std::to_string(this->deduceSymbolRate()));
    utils::printDataPoint("FEC inner", this->fec_inner, indent_level, this->deduceFECScheme());
    utils::printLine("Satellite delivery system descriptor", indent_level, '=');
}

double Descriptors::SatelliteDeliverySystemDescriptor::deduceFrequencyGHz() const {

    std::uint32_t value = 0;

    for (std::size_t i = 0; i < 8; i++) {
        std::uint8_t new_digit = (this->frequency >> (28 - i * 4)) & 0b1111;
        if(new_digit > 9) throw std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce the frequency in GHz: Invalid frequency bitfield.");

        value = value * 10 + new_digit;
    }

    return static_cast<double>(value) / 100'000.0;
}

float Descriptors::SatelliteDeliverySystemDescriptor::deduceOrbitalPositionInDegrees() const {

    std::uint16_t value = 0;

    for (std::size_t i = 0; i < 4; i++) {
        std::uint8_t new_digit = (this->frequency >> (12 >> i * 4)) & 0b1111;
        if(new_digit > 9) throw std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce the orbital position in degrees: Invalid orbital position bitfield.");

        value = value * 10 + new_digit;
    }

    if (value > 360) throw std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce the orbital position in degrees: Angle larger than 360 degrees.");
    return static_cast<float>(value) / 10.0;
}

char Descriptors::SatelliteDeliverySystemDescriptor::deduceWestEast() const {
    return this->west_east_flag == false ? 'W' : 'E';
}

std::string Descriptors::SatelliteDeliverySystemDescriptor::deducePolarization() const {
    switch(this->polarization) {
        case 0: return "Linear - Horizontal";
        case 1: return "Linear - Vertical";
        case 2: return "Circular - Left";
        case 3: return "Circular - Right";
        default: throw std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce polarization: Invalid polarization value.");
    }
}

float Descriptors::SatelliteDeliverySystemDescriptor::deduceRollOff() const {

    if (this->modulation_system == 0) return -1.0;

    switch(this->roll_off) {
        case 0: return 0.35;
        case 1: return 0.25;
        case 2: return 0.2;
        default: return -1.0;
    }
}

std::string Descriptors::SatelliteDeliverySystemDescriptor::deduceModulationSystem() const {
    return this->modulation_system == 0? "DVB-S" : "DVB-S2";
}

std::string Descriptors::SatelliteDeliverySystemDescriptor::deduceModulationType() const {
    switch(this->modulation_type) {
        case 0: return "Auto";
        case 1: return "QPSK";
        case 2: return "8PSK";
        case 3: return this->modulation_system == 0? "16-QAM" : "N/A";
        default: throw std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce the modulation type: Invalid modulation type bitfield value.");
    }
}

double Descriptors::SatelliteDeliverySystemDescriptor::deduceSymbolRate() const {

    uint32_t value = 0;
    
    for (std::size_t i = 0; i < 7; i++) {
        /*
            this->symbol_rate is shifted right 4 bits when it was parsed,
            so i need to start 4 bits from the beginning (24 bit shift as opposed to 28)
        */
        std::uint8_t new_digit = (this->symbol_rate >> (24 - i * 4)) & 0b1111;
        if (new_digit > 9) std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce the symbol rate: Invalid symbol rate bitfield.");

        value = value * 10 + new_digit;
    }

    return static_cast<double>(value) / 10'000.0;
}

std::string Descriptors::SatelliteDeliverySystemDescriptor::deduceFECScheme() const {
    if (this->fec_inner > 15) throw std::runtime_error("[SatelliteDeliverySystemDescriptor] Failed to deduce the FEC scheme: Invalid fec value.");
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