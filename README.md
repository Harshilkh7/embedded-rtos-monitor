# Real-Time ESP32 RTOS Monitoring & Multi-Protocol Communication System

A simulation-first **ESP32 + FreeRTOS** embedded monitoring platform demonstrating real-time task design, ADC acquisition, UART telemetry, I²C/SPI peripheral initialization, fault detection, watchdog supervision, and automated validation.

> **Portfolio status:** the host simulator, validation suite and ESP32 firmware build are runnable without physical hardware. The firmware targets an ESP32 DevKit through PlatformIO and uses the FreeRTOS scheduler supplied by the ESP32 Arduino framework.

## Architecture

```text
ESP32 ADC / internal temperature
            |
            v
+-----------------------------+
| FreeRTOS Application        |
|-----------------------------|
| Sensor Task                 |
| Communication Task          |
| Monitor / Watchdog Task     |
| Queue-based IPC             |
+---------------+-------------+
                |
          UART telemetry
                |
                v
+-----------------------------+
| Python Host Monitor         |
| CRC decoder + analytics     |
+-----------------------------+

I²C / SPI -> peripheral bus initialization
Fault Manager -> NORMAL / WARNING / CRITICAL
Watchdog -> task liveness supervision
```

## Features

- Native **ESP32 DevKit firmware** built with PlatformIO and Arduino-ESP32
- FreeRTOS tasks with explicit priorities, stack sizes and queue-based IPC
- ADC-based voltage/current acquisition plus ESP32 internal temperature trend monitoring
- UART telemetry using a fixed binary frame and **CRC-8** integrity check
- I²C and SPI peripheral bus initialization at the platform boundary
- Fault-state machine: `NORMAL`, `WARNING`, `CRITICAL`
- Task heartbeat monitoring and ESP32 task-watchdog supervision
- Python host monitor and deterministic virtual MCU
- Protocol round-trip, corruption, fault and watchdog tests with pytest
- Wokwi ESP32 simulation configuration
- GitHub Actions CI for both Python validation and ESP32 firmware compilation
- Browser dashboard under `docs/`

## Repository layout

```text
firmware/
  inc/                 Firmware interfaces
  src/                 FreeRTOS application + ESP32 drivers
  platformio.ini       Canonical ESP32 build configuration
  Makefile             PlatformIO convenience wrapper

host/
  monitor.py           Host telemetry decoder + analytics
  simulator.py         Deterministic virtual MCU

protocol/
  telemetry.py         Packet format + CRC-8

tests/
  test_protocol.py
  test_monitor.py
  test_faults.py

docs/
  index.html            Interactive project dashboard
  architecture.md       Detailed architecture notes

simulation/wokwi/
  diagram.json          ESP32 + sensor simulation
  wokwi.toml            Firmware/ELF mapping

.github/workflows/
  ci.yml                Python tests + ESP32 firmware build
```

## Build the ESP32 firmware

Install PlatformIO, then:

```bash
cd firmware
pio run
```

Or:

```bash
make
```

PlatformIO's `esp32dev` environment targets the Espressif ESP32 Dev Module. The same firmware is used by the Wokwi simulation after building.

## Run the validation suite

```bash
python -m pip install -r requirements.txt
PYTHONPATH=. pytest -q
```

## Run the host simulation

```bash
python tools/run_demo.py
```

## Open the dashboard

Open `docs/index.html` locally in a browser, or publish the `docs/` directory with GitHub Pages.

## Resume description

> Developed a real-time ESP32 monitoring system using C/C++ and FreeRTOS task architecture, ADC acquisition, queue-based IPC, UART telemetry, I²C/SPI interfaces, CRC-protected communication, watchdog supervision, fault-state management, Python simulation, Wokwi modeling, and automated pytest validation.

## Engineering honesty

No physical hardware measurements are claimed. The ESP32 firmware is buildable and simulated through Wokwi; physical-board flashing and electrical measurements remain separate validation steps.
