from protocol.telemetry import Telemetry, decode


class TelemetryMonitor:
    def __init__(self):
        self.samples = []
        self.errors = 0

    def consume(self, frame: bytes) -> Telemetry | None:
        try:
            sample = decode(frame)
        except ValueError:
            self.errors += 1
            return None
        self.samples.append(sample)
        return sample

    @property
    def latest(self):
        return self.samples[-1] if self.samples else None

    def summary(self):
        if not self.samples:
            return {"samples": 0, "errors": self.errors}
        voltages = [s.voltage_v for s in self.samples]
        currents = [s.current_a for s in self.samples]
        return {
            "samples": len(self.samples),
            "errors": self.errors,
            "average_voltage_v": sum(voltages) / len(voltages),
            "peak_current_a": max(currents),
            "critical_faults": sum(s.fault_code == 2 for s in self.samples),
        }
