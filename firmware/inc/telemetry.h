#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t sequence;
    uint16_t voltage_mv;
    uint16_t current_ma;
    int16_t temperature_centi_c;
    uint8_t fault_code;
} telemetry_sample_t;

uint8_t telemetry_crc8(const uint8_t *data, uint16_t length);
uint16_t telemetry_encode(const telemetry_sample_t *sample,
                          uint8_t *frame, uint16_t capacity);

#ifdef __cplusplus
}
#endif

#endif
