from host.monitor import TelemetryMonitor
from host.simulator import VirtualMCU
from protocol.telemetry import encode


def main():
    mcu = VirtualMCU()
    monitor = TelemetryMonitor()

    for step in range(20):
        sample = mcu.tick(step)
        monitor.consume(encode(sample))

    print("Telemetry monitoring demo")
    print("-------------------------")
    for key, value in monitor.summary().items():
        print(f"{key}: {value}")


if __name__ == "__main__":
    main()
