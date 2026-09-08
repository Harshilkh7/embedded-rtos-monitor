#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <esp_task_wdt.h>
#include "platform.h"

namespace {
constexpr uint8_t VOLTAGE_ADC_PIN = 34;
constexpr uint8_t CURRENT_ADC_PIN = 35;
constexpr uint8_t I2C_SDA = 21;
constexpr uint8_t I2C_SCL = 22;
constexpr uint8_t SPI_SCK = 18;
constexpr uint8_t SPI_MISO = 19;
constexpr uint8_t SPI_MOSI = 23;
constexpr uint8_t SPI_CS = 5;
constexpr float CURRENT_FULL_SCALE_A = 3.00f;
}

void platform_init(void) {
    Serial.begin(115200);
    analogReadResolution(12);
    analogSetPinAttenuation(VOLTAGE_ADC_PIN, ADC_11db);
    analogSetPinAttenuation(CURRENT_ADC_PIN, ADC_11db);

    Wire.begin(I2C_SDA, I2C_SCL, 100000);

    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
}

uint16_t adc_read_voltage_mv(void) {
    return static_cast<uint16_t>(analogReadMilliVolts(VOLTAGE_ADC_PIN));
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

uint8_t i2c_probe(uint8_t address) {
    Wire.beginTransmission(address);
    return Wire.endTransmission() == 0 ? 1U : 0U;
}

uint8_t spi_transfer(uint8_t data) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(SPI_CS, LOW);
    const uint8_t response = SPI.transfer(data);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
    return response;
}
