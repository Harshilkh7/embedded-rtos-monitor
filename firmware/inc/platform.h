#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

void platform_init(void);
uint16_t adc_read_voltage_mv(void);
uint16_t adc_read_current_ma(void);
uint16_t adc_read_temperature_c(void);
void uart_send(const uint8_t *data, uint16_t length);
void watchdog_kick(void);

#endif
