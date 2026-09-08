#include <Arduino.h>
#include <esp_task_wdt.h>
#include "platform.h"

namespace {
constexpr uint8_t VOLTAGE_ADC_PIN = 34;
constexpr uint8_t CURRENT_ADC_PIN = 35;
constexpr float CURRENT_FULL_SCALE_A = 3.00f;
}

void platform_init(void) {
    Serial.begin(115200);
    analogReadResolution(12);
    analogSetPinAttenuation(VOLTAGE_ADC_PIN, ADC_11db);
    analogSetPinAttenuation(CURRENT_ADC_PIN, ADC_11db);
}

uint16_t adc_read_voltage_mv(void) {
    const uint16_t raw = static_cast<uint16_t>(analogRead(VOLTAGE_ADC_PIN));
    return static_cast<uint16_t>((static_cast<uint32_t>(raw) * 3300U) / 4095U);
}

uint16_t adc_read_current_ma(void) {
    const uint16_t raw = static_cast<uint16_t>(analogRead(CURRENT_ADC_PIN));
    const float current = (static_cast<float>(raw) / 4095.0f) * CURRENT_FULL_SCALE_A;
    return static_cast<uint16_t>(current * 1000.0f);
}

float adc_read_temperature_c(void) {
    // Internal silicon temperature is useful for trend monitoring, not
    // precision ambient measurement.
    return temperatureRead();
}

void uart_send(const uint8_t *data, uint16_t length) {
    Serial.write(data, length);
}

void watchdog_kick(void) {
    // The calling RTOS task must first be subscribed with esp_task_wdt_add().
    (void)esp_task_wdt_reset();
}
