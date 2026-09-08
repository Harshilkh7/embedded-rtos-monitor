import pytest
from protocol.telemetry import Telemetry, encode, decode


def test_round_trip():
    sample = Telemetry(42, 3.30, 0.75, 36.5, 0)
    assert decode(encode(sample)) == sample


def test_crc_detects_corruption():
    frame = bytearray(encode(Telemetry(1, 3.3, 0.5, 35.0)))
    frame[-2] ^= 0x01
    with pytest.raises(ValueError, match="CRC"):
        decode(bytes(frame))


def test_bad_length_rejected():
    frame = bytearray(encode(Telemetry(1, 3.3, 0.5, 35.0)))
    frame[1] += 1
    with pytest.raises(ValueError, match="length"):
        decode(bytes(frame))
