from dataclasses import dataclass
from protocol.telemetry import Telemetry

NORMAL = 0
WARNING = 1
CRITICAL = 2


@dataclass
class SensorModel:
    voltage_v: float = 3.30
    current_a: float = 0.50
    temperature_c: float = 35.0

    def sample(self, step: int) -> Telemetry:
        voltage = self.voltage_v + 0.02 * ((step % 10) - 5) / 5
        current = self.current_a + 0.04 * ((step % 8) - 4) / 4
        temperature = self.temperature_c + 0.5 * ((step % 6) - 3)
        fault = classify_fault(voltage, current, temperature)
        return Telemetry(step & 0xFFFF, voltage, current, temperature, fault)


def classify_fault(voltage_v: float, current_a: float, temperature_c: float) -> int:
    if voltage_v < 2.80 or voltage_v > 3.63 or current_a > 2.00 or temperature_c > 85.0:
        return CRITICAL
    if voltage_v < 3.00 or voltage_v > 3.50 or current_a > 1.50 or temperature_c > 70.0:
        return WARNING
    return NORMAL


class VirtualMCU:
    def __init__(self, sensor=None):
        self.sensor = sensor or SensorModel()
        self.watchdog_deadline = 3
        self.watchdog_counter = 0

    def tick(self, step: int) -> Telemetry:
        self.watchdog_counter = 0
        return self.sensor.sample(step)

    def missed_task_ticks(self, count: int) -> bool:
        self.watchdog_counter += count
        return self.watchdog_counter > self.watchdog_deadline
