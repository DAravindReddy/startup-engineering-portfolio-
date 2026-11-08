/**
 * @file pump_controller.cpp
 * @brief Water pump extinguishing relay controller implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "pump_controller.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
static void pinMode(uint8_t, uint8_t) {}
static void digitalWrite(uint8_t, uint8_t) {}
#define OUTPUT 1
#define HIGH   1
#define LOW    0
#endif

void pump_controller_init(PumpController_t *pump, uint8_t relay_pin) {
    if (!pump) return;

    pump->relay_pin = relay_pin;
    pump->is_active = false;
    pump->activation_time_ms = 0;
    pump->consecutive_spray_count = 0;

    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, LOW);
}

void pump_controller_start(PumpController_t *pump, uint32_t current_time_ms) {
    if (!pump) return;

    pump->is_active = true;
    pump->activation_time_ms = current_time_ms;
    pump->consecutive_spray_count++;
    digitalWrite(pump->relay_pin, HIGH);
}

void pump_controller_stop(PumpController_t *pump) {
    if (!pump) return;

    pump->is_active = false;
    digitalWrite(pump->relay_pin, LOW);
}

void pump_controller_update(PumpController_t *pump, uint32_t current_time_ms) {
    if (!pump || !pump->is_active) return;

    // Safety watchdog: automatically kill pump after EXTINGUISH_SPRAY_MS or PUMP_WATCHDOG_MAX_MS
    if (current_time_ms - pump->activation_time_ms >= EXTINGUISH_SPRAY_MS) {
        pump_controller_stop(pump);
    }
}

bool pump_controller_is_running(const PumpController_t *pump) {
    return pump ? pump->is_active : false;
}
