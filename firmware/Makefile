# ESP32 build/organization entry point.
# The application is intentionally kept portable and RTOS-oriented; the
# FreeRTOS kernel/toolchain integration is supplied by the target environment.

TARGET := embedded_monitor
BUILD := build

CFLAGS := -std=c11 -Wall -Wextra -O2
CFLAGS += -Ifirmware/inc -Ithird_party/FreeRTOS-Kernel/include
CFLAGS += -Ithird_party/FreeRTOS-Kernel/portable/GCC/ESP32

SRCS := firmware/src/main.c firmware/src/fault_manager.c firmware/src/telemetry.c firmware/src/platform_stub.c

all:
	@echo "ESP32 + FreeRTOS application sources:"
	@printf '  %s\n' $(SRCS)
	@echo "Provide the FreeRTOS kernel and ESP32 toolchain in the target environment for a hardware build."

clean:
	rm -rf $(BUILD)
