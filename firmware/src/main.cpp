#include <Arduino.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "app_config.h"
#include "fault_manager.h"
#include "platform.h"
#include "telemetry.h"

static QueueHandle_t sensor_queue;
static volatile uint32_t sensor_heartbeat = 0;
static volatile uint32_t communication_heartbeat = 0;
static volatile bool watchdog_fault = false;

static void sensor_task(void *argument) {
    (void)argument;
    uint16_t sequence = 0;

    for (;;) {
        telemetry_sample_t sample{};
        sample.sequence = sequence++;
        sample.voltage_mv = adc_read_voltage_mv();
        sample.current_ma = adc_read_current_ma();
        sample.temperature_centi_c = static_cast<int16_t>(adc_read_temperature_c() * 100U);
        sample.fault_code = static_cast<uint8_t>(fault_classify(
            sample.voltage_mv,
            sample.current_ma,
            static_cast<uint16_t>(sample.temperature_centi_c / 100)));

        if (xQueueSend(sensor_queue, &sample, pdMS_TO_TICKS(10)) == pdPASS) {
            ++sensor_heartbeat;
        }
        vTaskDelay(pdMS_TO_TICKS(TELEMETRY_PERIOD_MS));
    }
}

static void communication_task(void *argument) {
    (void)argument;
    telemetry_sample_t sample{};
    uint8_t frame[16];

    for (;;) {
        if (xQueueReceive(sensor_queue, &sample, portMAX_DELAY) == pdPASS) {
            const uint16_t length = telemetry_encode(&sample, frame, sizeof(frame));
            if (length > 0U) {
                uart_send(frame, length);
                ++communication_heartbeat;
            }
        }
    }
}

static void monitor_task(void *argument) {
    (void)argument;
    uint32_t last_sensor = sensor_heartbeat;
    uint32_t last_communication = communication_heartbeat;

    // Subscribe this RTOS task to the ESP32 task watchdog. The watchdog is
    // intentionally fed only from this task so a stuck monitor task is fatal.
    (void)esp_task_wdt_add(nullptr);

    for (;;) {
        watchdog_kick();
        const bool sensor_alive = sensor_heartbeat != last_sensor;
        const bool communication_alive = communication_heartbeat != last_communication;

        if (!sensor_alive || !communication_alive) {
            watchdog_fault = true;
            Serial.printf("WATCHDOG_MONITOR: sensor=%s comm=%s\r\n",
                          sensor_alive ? "OK" : "STALE",
                          communication_alive ? "OK" : "STALE");
        } else {
            watchdog_fault = false;
        }

        last_sensor = sensor_heartbeat;
        last_communication = communication_heartbeat;
        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}

void setup() {
    platform_init();

    sensor_queue = xQueueCreate(SENSOR_QUEUE_LENGTH, sizeof(telemetry_sample_t));
    if (sensor_queue == nullptr) {
        Serial.println("FATAL: queue allocation failed");
        for (;;) {
            delay(1000);
        }
    }

    xTaskCreatePinnedToCore(sensor_task, "sensor", 3072, nullptr, 3, nullptr, 1);
    xTaskCreatePinnedToCore(communication_task, "comm", 3072, nullptr, 2, nullptr, 1);
    xTaskCreatePinnedToCore(monitor_task, "monitor", 3072, nullptr, 4, nullptr, 0);

    Serial.println("ESP32 FreeRTOS monitoring firmware started");
}

void loop() {
    // Application work is performed by FreeRTOS tasks. Keep the Arduino loop
    // cooperative so the framework's scheduler and idle task continue running.
    delay(1000);
}
