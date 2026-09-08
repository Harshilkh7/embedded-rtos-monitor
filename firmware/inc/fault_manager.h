#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include <stdint.h>

typedef enum {
    FAULT_NORMAL = 0,
    FAULT_WARNING = 1,
    FAULT_CRITICAL = 2
} fault_state_t;

fault_state_t fault_classify(uint16_t voltage_mv,
                             uint16_t current_ma,
                             uint16_t temperature_c);

#endif
