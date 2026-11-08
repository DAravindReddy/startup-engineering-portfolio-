/**
 * @file pump_controller.h
 * @brief Water pump extinguishing relay controller with watchdog protection.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t relay_pin;
    bool is_active;
    uint32_t activation_time_ms;
    uint8_t consecutive_spray_count;
} PumpController_t;

void pump_controller_init(PumpController_t *pump, uint8_t relay_pin);

void pump_controller_start(PumpController_t *pump, uint32_t current_time_ms);

void pump_controller_stop(PumpController_t *pump);

void pump_controller_update(PumpController_t *pump, uint32_t current_time_ms);

bool pump_controller_is_running(const PumpController_t *pump);

#ifdef __cplusplus
}
#endif

#endif // PUMP_CONTROLLER_H
