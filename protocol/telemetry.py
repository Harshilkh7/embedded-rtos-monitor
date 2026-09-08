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
    voltage_mv = int(round(sample.voltage_v * 1000))
    current_ma = int(round(sample.current_a * 1000))
    temperature_cc = int(round(sample.temperature_c * 100))
    if not 0 <= sample.sequence <= 0xFFFF:
        raise ValueError("sequence out of range")
    if not 0 <= voltage_mv <= 0xFFFF or not 0 <= current_ma <= 0xFFFF:
        raise ValueError("unsigned telemetry value out of range")
    if not -32768 <= temperature_cc <= 32767:
        raise ValueError("temperature out of int16 range")

    temperature_u16 = temperature_cc & 0xFFFF
    payload = bytes([
        PROTOCOL_VERSION,
        (sample.sequence >> 8) & 0xFF,
        sample.sequence & 0xFF,
        voltage_mv >> 8,
        voltage_mv & 0xFF,
        current_ma >> 8,
        current_ma & 0xFF,
        (temperature_u16 >> 8) & 0xFF,
        temperature_u16 & 0xFF,
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
    temperature_u16 = (payload[7] << 8) | payload[8]
    temperature_cc = temperature_u16 - 0x10000 if temperature_u16 & 0x8000 else temperature_u16
    return Telemetry(sequence, voltage_mv / 1000.0, current_ma / 1000.0,
                     temperature_cc / 100.0, payload[9])
