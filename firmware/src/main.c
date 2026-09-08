/* STM32 + FreeRTOS application skeleton. */
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "app_config.h"
#include "fault_manager.h"
#include "platform.h"
#include "telemetry.h"

static QueueHandle_t sensor_queue;

static void sensor_task(void *argument)
{
    (void)argument;
    uint16_t sequence = 0;
    for (;;) {
        telemetry_sample_t sample = {
            .sequence = sequence++,
            .voltage_mv = adc_read_voltage_mv(),
            .current_ma = adc_read_current_ma(),
            .temperature_centi_c = (int16_t)(adc_read_temperature_c() * 100U),
            .fault_code = FAULT_NORMAL
        };
        sample.fault_code = (uint8_t)fault_classify(
            sample.voltage_mv, sample.current_ma,
            (uint16_t)(sample.temperature_centi_c / 100));
        (void)xQueueSend(sensor_queue, &sample, 0);
        vTaskDelay(pdMS_TO_TICKS(TELEMETRY_PERIOD_MS));
    }
}

static void communication_task(void *argument)
{
    (void)argument;
    telemetry_sample_t sample;
    uint8_t frame[16];
    for (;;) {
        if (xQueueReceive(sensor_queue, &sample, portMAX_DELAY) == pdPASS) {
            uint16_t length = telemetry_encode(&sample, frame, sizeof(frame));
            if (length > 0U) uart_send(frame, length);
        }
    }
}

static void monitor_task(void *argument)
{
    (void)argument;
    for (;;) {
        watchdog_kick();
        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}

int main(void)
{
    platform_init();
    sensor_queue = xQueueCreate(SENSOR_QUEUE_LENGTH, sizeof(telemetry_sample_t));
    if (sensor_queue == NULL) {
        for (;;) {}
    }
    xTaskCreate(sensor_task, "sensor", 256, NULL, 3, NULL);
    xTaskCreate(communication_task, "comm", 256, NULL, 2, NULL);
    xTaskCreate(monitor_task, "monitor", 256, NULL, 4, NULL);
    vTaskStartScheduler();
    for (;;) {}
}
