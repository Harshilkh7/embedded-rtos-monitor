from dataclasses import dataclass

START_BYTE = 0xAA
PROTOCOL_VERSION = 1


def crc8(data: bytes) -> int:
    crc = 0
    for value in data:
        crc ^= value
        for _ in range(8):
            crc = ((crc << 1) ^ 0x07) & 0xFF if crc & 0x80 else (crc << 1) & 0xFF
    return crc


@dataclass(frozen=True)
class Telemetry:
    sequence: int
    voltage_v: float
    current_a: float
    temperature_c: float
    fault_code: int = 0


def encode(sample: Telemetry) -> bytes:
    payload = bytes([
        PROTOCOL_VERSION,
        (sample.sequence >> 8) & 0xFF,
        sample.sequence & 0xFF,
        int(round(sample.voltage_v * 1000)) >> 8,
        int(round(sample.voltage_v * 1000)) & 0xFF,
        int(round(sample.current_a * 1000)) >> 8,
        int(round(sample.current_a * 1000)) & 0xFF,
        (int(round(sample.temperature_c * 100)) >> 8) & 0xFF,
        int(round(sample.temperature_c * 100)) & 0xFF,
        sample.fault_code & 0xFF,
    ])
    return bytes([START_BYTE, len(payload)]) + payload + bytes([crc8(payload)])


def decode(frame: bytes) -> Telemetry:
    if len(frame) < 4 or frame[0] != START_BYTE:
        raise ValueError("invalid frame header")
    length = frame[1]
    if len(frame) != length + 3:
        raise ValueError("invalid frame length")
    payload = frame[2:-1]
    if frame[-1] != crc8(payload):
        raise ValueError("CRC mismatch")
    if len(payload) != 10 or payload[0] != PROTOCOL_VERSION:
        raise ValueError("unsupported payload")
    sequence = (payload[1] << 8) | payload[2]
    voltage_mv = (payload[3] << 8) | payload[4]
    current_ma = (payload[5] << 8) | payload[6]
    temperature_cc = (payload[7] << 8) | payload[8]
    return Telemetry(sequence, voltage_mv / 1000.0, current_ma / 1000.0,
                     temperature_cc / 100.0, payload[9])
