#ifndef _TSPACKET_H
#define _TSPACKET_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <optional>
#include <memory>

struct TSPacket {

    struct TSPacketHeader {
        std::uint8_t sync_byte;
        bool transport_error_indicator;
        bool payload_unit_start_indicator;
        bool transport_priority;
        std::uint16_t pid;
        std::uint8_t transport_scrambling_control;
        std::uint8_t adaptation_field_control;
        std::uint8_t continuity_counter;

        static constexpr uint8_t size = 4;

        TSPacketHeader(const std::uint8_t* start);

        TSPacketHeader() = delete;
        TSPacketHeader(const TSPacketHeader&) = delete;
        TSPacketHeader& operator=(const TSPacketHeader&) = delete;

        TSPacketHeader(TSPacketHeader&&) = default;
        TSPacketHeader& operator=(TSPacketHeader&&) = default;
        ~TSPacketHeader() = default;
    };

    struct AdaptationField {

        struct AdaptationExtension {
            
            struct LegalTimeWindow {
                bool ltw_valid_flag;
                std::uint16_t ltw_offset;

                LegalTimeWindow(const std::uint8_t* start) 
                    : ltw_valid_flag((start[0] >> 7) & 0b1),
                    ltw_offset(((start[0] & 0b01111111) << 8) | start[1]) {}
            };


            struct SeamlessSplice {
                std::uint8_t splice_type;
                std::uint64_t dts_next_access_unit;

                SeamlessSplice(std::uint8_t* start)
                    : splice_type((start[0] >> 4) & 0b1111),
                    dts_next_access_unit(
                        (static_cast<std::uint64_t>(start[0] & 0b1111) << 32)
                        | (static_cast<std::uint64_t>(start[1]) << 24)
                        | (static_cast<std::uint64_t>(start[2]) << 16)
                        | (static_cast<std::uint64_t>(start[3]) << 8)
                        | start[4]
                    ) 
                    {}
            };

        private:

            std::uint8_t adaptation_extension_length;
            std::optional<bool> legal_time_window_flag;
            std::optional<bool> piecewise_rate_flag;
            std::optional<bool> seamless_splice_flag;

            std::optional<LegalTimeWindow> legal_time_window;
            std::optional<std::uint32_t> piecewise_rate;
            std::optional<SeamlessSplice> seamless_splice;

        public:

            AdaptationExtension(const std::uint8_t length, const std::uint8_t* start);

            AdaptationExtension() = delete;
            AdaptationExtension(const AdaptationExtension&) = delete;
            AdaptationExtension& operator=(const AdaptationExtension&) = delete;

            AdaptationExtension(AdaptationExtension&&) = default;
            AdaptationExtension& operator=(AdaptationExtension&&) = default;
            ~AdaptationExtension() = default;
        };


        class TransportPrivateData {
            std::uint8_t length;
            std::vector<std::uint8_t> data;
        public:
            TransportPrivateData(std::uint8_t length, std::uint8_t* start)
                : length(length), data(start, start + length) {}
        };

        std::uint8_t adaptation_field_length;
        std::optional<bool> discontinuity_indicator;
        std::optional<bool> random_access_indicator;
        std::optional<bool> elementary_stream_priority_indicator;
        std::optional<bool> pcr_flag;
        std::optional<bool> opcr_flag;
        std::optional<bool> splicing_point_flag;
        std::optional<bool> transport_private_data_flag;
        std::optional<bool> adaptation_field_extension_flag;

        std::optional<std::uint64_t> pcr;
        std::optional<std::uint64_t> opcr;
        std::optional<std::int8_t> splice_countdown; // Can be negative to indicate packets in the past
        std::optional<TransportPrivateData> transport_private_data;
        std::optional<AdaptationExtension> adaptation_extension;
        
        std::unique_ptr<std::uint8_t[]> bit_representation;
    

        AdaptationField(const std::uint8_t length, const std::uint8_t* start);

        AdaptationField() = delete;
        AdaptationField(const AdaptationField&) = delete;
        AdaptationField& operator=(const AdaptationField&) = delete;

        AdaptationField(AdaptationField&&) = default;
        AdaptationField& operator=(AdaptationField&&) = default;
        ~AdaptationField() = default;
    };

    TSPacketHeader header;
    std::optional<AdaptationField> adaptation_field;
    std::optional<std::vector<std::uint8_t>> payload;

public:

    void print(const std::size_t indent_level) const;
    inline std::uint8_t getAdaptationFieldControl() const {
        return this->header.adaptation_field_control;
    }

    TSPacket(const std::uint8_t* start, const std::uint8_t packet_size); 
    
    TSPacket() = delete;
    TSPacket(const TSPacket&) = delete;
    TSPacket& operator=(const TSPacket&) = delete;

    TSPacket(TSPacket&&) = default;
    TSPacket& operator=(TSPacket&&) = default;
    ~TSPacket() = default;
};

#endif // _TSPACKET_H