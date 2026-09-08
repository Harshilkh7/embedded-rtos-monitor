from host.monitor import TelemetryMonitor
from host.simulator import SensorModel
from protocol.telemetry import encode


def test_monitor_collects_samples():
    monitor = TelemetryMonitor()
    sensor = SensorModel()
    for step in range(5):
        monitor.consume(encode(sensor.sample(step)))
    result = monitor.summary()
    assert result["samples"] == 5
    assert result["errors"] == 0
    assert result["peak_current_a"] > 0


def test_monitor_rejects_bad_frame():
    monitor = TelemetryMonitor()
    assert monitor.consume(b"\x00\x01") is None
    assert monitor.errors == 1
