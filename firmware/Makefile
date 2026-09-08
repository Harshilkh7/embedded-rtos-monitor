# Convenience wrapper around the canonical PlatformIO ESP32 build.

.PHONY: all clean test

all:
	pio run

test:
	cd .. && python -m pytest -q

clean:
	pio run --target clean
