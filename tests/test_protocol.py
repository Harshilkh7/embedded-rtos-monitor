from protocol.telemetry import Telemetry, crc8, decode, encode


def test_crc_known_vector_is_stable():
    assert crc8(b"ABC") == 0x52


def test_round_trip():
    sample = Telemetry(42, 3.30, 0.50, 35.25, 0)
    assert decode(encode(sample)) == sample


def test_negative_temperature_round_trip():
    sample = Telemetry(7, 3.10, 1.20, -5.50, 1)
    assert decode(encode(sample)) == sample


def test_corruption_rejected():
    frame = bytearray(encode(Telemetry(1, 3.3, 0.5, 35.0)))
    frame[-1] ^= 0xFF
    try:
        decode(bytes(frame))
    except ValueError:
        return
    assert False


def test_header_and_length_validation():
    frame = encode(Telemetry(1, 3.3, 0.5, 35.0))
    try:
        decode(b"\x00" + frame[1:])
    except ValueError:
        pass
    else:
        assert False

    try:
        decode(frame[:-1])
    except ValueError:
        pass
    else:
        assert False
