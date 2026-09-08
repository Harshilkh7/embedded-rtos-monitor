#include "telemetry.h"

#define START_BYTE 0xAAU
#define PROTOCOL_VERSION 1U
#define PAYLOAD_LEN 10U

uint8_t telemetry_crc8(const uint8_t *data, uint16_t length)
{
    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; ++bit) {
            crc = (crc & 0x80U)
                ? (uint8_t)((crc << 1) ^ 0x07U)
                : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

uint16_t telemetry_encode(const telemetry_sample_t *sample,
                          uint8_t *frame, uint16_t capacity)
{
    if (capacity < PAYLOAD_LEN + 3U) {
        return 0U;
    }

    frame[0] = START_BYTE;
    frame[1] = PAYLOAD_LEN;
    frame[2] = PROTOCOL_VERSION;
    frame[3] = (uint8_t)(sample->sequence >> 8);
    frame[4] = (uint8_t)sample->sequence;
    frame[5] = (uint8_t)(sample->voltage_mv >> 8);
    frame[6] = (uint8_t)sample->voltage_mv;
    frame[7] = (uint8_t)(sample->current_ma >> 8);
    frame[8] = (uint8_t)sample->current_ma;
    frame[9] = (uint8_t)(sample->temperature_centi_c >> 8);
    frame[10] = (uint8_t)sample->temperature_centi_c;
    frame[11] = sample->fault_code;
    frame[12] = telemetry_crc8(&frame[2], PAYLOAD_LEN);
    return PAYLOAD_LEN + 3U;
}
