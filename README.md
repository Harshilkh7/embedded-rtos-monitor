# Real-Time Embedded Monitoring & Multi-Protocol Communication System

A simulation-first STM32/FreeRTOS-oriented embedded monitoring platform demonstrating real-time task design, ADC acquisition, UART telemetry, I2C/SPI device interfaces, fault detection, watchdog supervision, and automated validation.

> **Portfolio status:** The host-side simulator and validation suite are runnable without hardware. The MCU application is structured for an STM32F103 + FreeRTOS target; the official FreeRTOS Kernel is intentionally kept as an external dependency rather than copied into this repository.

## Architecture

```text
Virtual Sensors
      |
      v
+-----------------------+
| STM32 / FreeRTOS App  |
|-----------------------|
| Sensor Task           |
| Communication Task    |
| Monitor Task          |
| Watchdog Supervision  |
+----------+------------+
           |
     UART telemetry
           |
           v
+-----------------------+
| Python Host Monitor   |
| protocol + dashboard  |
+-----------------------+

I2C / SPI are modeled as peripheral-device interfaces.
ADC models voltage/current/temperature acquisition.
```

## Features

- C firmware organized around RTOS tasks and queues
- ADC-based voltage/current/temperature acquisition
- UART telemetry with framed packets and CRC-8
- I2C sensor abstraction
- SPI device abstraction
- Fault-state machine: `NORMAL`, `WARNING`, `CRITICAL`
- Watchdog supervision model
- Python host monitor and deterministic simulator
- Pytest validation
- GitHub Actions CI
- Browser dashboard under `docs/`

## Repository layout

```text
firmware/
  inc/                 Firmware interfaces
  src/                 RTOS application and drivers
  Makefile             STM32 build entry point

host/
  monitor.py           Host telemetry decoder
  simulator.py         Deterministic virtual MCU

protocol/
  telemetry.py         Packet format + CRC

tests/
  test_protocol.py
  test_monitor.py
  test_faults.py

docs/
  index.html            Interactive project dashboard
  architecture.md       Detailed architecture notes

.github/workflows/
  ci.yml                Automated Python validation

tools/
  run_demo.py            CLI demonstration
```

## Run the validation suite

```bash
python -m pip install -r requirements.txt
pytest -q
```

## Run the host simulation

```bash
python tools/run_demo.py
```

## Open the dashboard

Open `docs/index.html` locally in a browser, or publish the `docs/` directory with GitHub Pages after the repository is created.

## MCU dependency

The firmware application expects the [FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) to be supplied under:

```text
third_party/FreeRTOS-Kernel/
```

The repository deliberately does not vendor the kernel source. This keeps the portfolio project focused on application-level embedded engineering.

## Resume description

> Developed a real-time STM32 monitoring system using C and FreeRTOS-style task architecture, ADC acquisition, UART telemetry, I2C/SPI device interfaces, CRC-protected communication, watchdog supervision, fault-state management, Python simulation, and automated pytest validation.

## Engineering honesty

The current repository provides a **runnable host simulation and validation environment** plus MCU application scaffolding. It should not be described as hardware-tested until the firmware is built, flashed, and verified on an STM32 board.
