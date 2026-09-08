TARGET := embedded_monitor
BUILD := build
CC := arm-none-eabi-gcc

CFLAGS := -mcpu=cortex-m3 -mthumb -std=c11 -Wall -Wextra -O2
CFLAGS += -Ifirmware/inc -Ithird_party/FreeRTOS-Kernel/include
CFLAGS += -Ithird_party/FreeRTOS-Kernel/portable/GCC/ARM_CM3

SRCS := firmware/src/main.c firmware/src/fault_manager.c firmware/src/telemetry.c firmware/src/platform_stub.c

all:
	@echo "FreeRTOS kernel is an external dependency."
	@echo "Place it under third_party/FreeRTOS-Kernel and add startup/linker sources before a hardware build."
	@echo "Application sources: $(SRCS)"

clean:
	rm -rf $(BUILD)
