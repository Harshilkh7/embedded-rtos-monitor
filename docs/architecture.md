# Architecture

## Runtime tasks

### Sensor Task
Runs periodically, samples ESP32 ADC channels and the internal temperature sensor, classifies the latest sample, and publishes a fixed-size telemetry record to a FreeRTOS queue.

### Communication Task
Blocks on the sensor queue, serializes telemetry into a framed UART packet with a CRC-8 checksum, and sends the frame to the host monitor.

### Monitor Task
Runs at a lower rate, subscribes to the ESP32 task watchdog, feeds it only from the monitor task, and checks sensor/communication heartbeats for stale tasks.

## Inter-task communication

The sensor and communication tasks exchange `telemetry_sample_t` records through a FreeRTOS queue. This avoids sharing mutable sensor state directly and gives the communication task a deterministic ownership boundary.

## ESP32 peripheral layer

- **ADC:** GPIO 34 voltage sensing and GPIO 35 current sensing.
- **Temperature:** ESP32 internal silicon-temperature reading for trend monitoring.
- **UART:** binary telemetry transport at 115200 baud.
- **I²C:** GPIO 21/22 bus initialization plus address-probe primitive.
- **SPI:** GPIO 18/19/23/5 bus initialization plus full-duplex transfer primitive.
- **Watchdog:** ESP32 task watchdog used by the monitor task.

## Fault model

| Condition | Warning | Critical |
|---|---:|---:|
| Voltage | <3.00 V or >3.50 V | <2.80 V or >3.63 V |
| Current | >1.50 A | >2.00 A |
| Temperature | >70 C | >85 C |

The Python simulator and C fault manager use the same thresholds so the host validation environment can exercise the embedded decision logic deterministically.

## Telemetry protocol

```text
0xAA | payload length | version | sequence | voltage_mV |
current_mA | temperature_centi_C | fault_code | CRC-8
```

CRC-8 uses polynomial `0x07` and is calculated over the version through fault-code bytes. The Python decoder and C encoder share the same frame layout.

## Validation boundary

The hardware-facing ESP32 platform layer is separated from the RTOS application logic. The host simulator and pytest suite validate fault decisions, telemetry framing and watchdog behavior without requiring a physical board for every run, while PlatformIO CI verifies that the actual ESP32 firmware compiles.
