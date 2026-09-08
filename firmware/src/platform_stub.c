/* ESP32 platform boundary for the RTOS-oriented application model. */
#include "platform.h"

void platform_init(void) {}
uint16_t adc_read_voltage_mv(void) { return 3300U; }
uint16_t adc_read_current_ma(void) { return 500U; }
uint16_t adc_read_temperature_c(void) { return 35U; }
void uart_send(const uint8_t *data, uint16_t length) { (void)data; (void)length; }
void watchdog_kick(void) {}
