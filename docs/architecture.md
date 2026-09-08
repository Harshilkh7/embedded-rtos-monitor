# Architecture

## Runtime tasks

### Sensor Task
Samples ADC channels and obtains temperature data. It classifies the latest sample and publishes a fixed-size telemetry record to a queue.

### Communication Task
Consumes telemetry records and serializes them into a framed UART packet with a CRC-8 checksum.

### Monitor Task
Runs at a lower rate and represents watchdog supervision. A production implementation would also observe task heartbeats and trigger a safe state when a task stops making progress.

## ESP32 peripheral model

- **ADC:** ESP32 voltage, current, and temperature acquisition boundary.
- **UART:** host telemetry transport.
- **I2C:** reserved for external digital sensors.
- **SPI:** reserved for high-rate peripheral or display/flash interface.
- **Watchdog:** recovery mechanism for software lockups.

## Fault model

| Condition | Warning | Critical |
|---|---:|---:|
| Voltage | <3.00 V or >3.50 V | <2.80 V or >3.63 V |
| Current | >1.50 A | >2.00 A |
| Temperature | >70 C | >85 C |

The Python simulator and C fault manager use the same thresholds so the host validation environment can exercise the embedded decision logic deterministically.

## Validation boundary

The repository separates the hardware-facing ESP32 platform boundary from the RTOS application logic. This lets the host simulator and pytest suite validate task behavior, telemetry framing, watchdog logic, and fault decisions without requiring a physical board for every run.
