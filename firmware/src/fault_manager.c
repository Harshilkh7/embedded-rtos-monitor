#include "fault_manager.h"
#include "app_config.h"

fault_state_t fault_classify(uint16_t voltage_mv,
                             uint16_t current_ma,
                             uint16_t temperature_c)
{
    if (voltage_mv < VOLTAGE_UV_LIMIT_MV ||
        voltage_mv > VOLTAGE_OV_LIMIT_MV ||
        current_ma > CURRENT_OC_LIMIT_MA ||
        temperature_c > TEMPERATURE_OT_LIMIT_C) {
        return FAULT_CRITICAL;
    }

    if (voltage_mv < 3000U ||
        voltage_mv > 3500U ||
        current_ma > 1500U ||
        temperature_c > 70U) {
        return FAULT_WARNING;
    }

    return FAULT_NORMAL;
}
