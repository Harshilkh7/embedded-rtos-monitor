#include "telemetry.h"

#define START_BYTE 0xAAU
#define PROTOCOL_VERSION 1U
#define PAYLOAD_LEN 10U

uint8_t telemetry_crc8(const uint8_t *data, uint16_t length)
{
    uint8_t crc = 0U;
    for (uint16_t i = 0U; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            crc = (crc & 0x80U)
                ? (uint8_t)((crc << 1U) ^ 0x07U)
                : (uint8_t)(crc << 1U);
        }
    }
    return crc;
}

uint16_t telemetry_encode(const telemetry_sample_t *sample,
                          uint8_t *frame, uint16_t capacity)
{
    if (sample == 0 || frame == 0 || capacity < PAYLOAD_LEN + 3U) {
        return 0U;
    }

    const uint16_t temperature_cc = (uint16_t)sample->temperature_centi_c;

    frame[0] = START_BYTE;
    frame[1] = PAYLOAD_LEN;
    frame[2] = PROTOCOL_VERSION;
    frame[3] = (uint8_t)(sample->sequence >> 8U);
    frame[4] = (uint8_t)sample->sequence;
    frame[5] = (uint8_t)(sample->voltage_mv >> 8U);
    frame[6] = (uint8_t)sample->voltage_mv;
    frame[7] = (uint8_t)(sample->current_ma >> 8U);
    frame[8] = (uint8_t)sample->current_ma;
    frame[9] = (uint8_t)(temperature_cc >> 8U);
    frame[10] = (uint8_t)temperature_cc;
    frame[11] = sample->fault_code;
    frame[12] = telemetry_crc8(&frame[2], PAYLOAD_LEN);
    return PAYLOAD_LEN + 3U;
}
