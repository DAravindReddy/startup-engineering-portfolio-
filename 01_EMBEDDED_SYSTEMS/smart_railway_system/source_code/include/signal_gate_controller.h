/**
 * @file signal_gate_controller.h
 * @brief Railway traffic signals, level-crossing barrier gate, and alert buzzer interface.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef SIGNAL_GATE_CONTROLLER_H
#define SIGNAL_GATE_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SIGNAL_ASPECT_GREEN = 0,  // Track Clear - Safe to Proceed
    SIGNAL_ASPECT_YELLOW,     // Caution - Obstacle in Outer Zone
    SIGNAL_ASPECT_RED         // Danger - Track Fracture or Critical Obstacle
} SignalAspect_t;

typedef struct {
    uint8_t pin_green;
    uint8_t pin_yellow;
    uint8_t pin_red;
    uint8_t pin_gate_servo;
    uint8_t pin_buzzer;
    SignalAspect_t current_aspect;
    uint8_t current_gate_angle;
    bool buzzer_active;
} SignalGateController_t;

void signal_gate_init(SignalGateController_t *ctrl, uint8_t pin_g, uint8_t pin_y,
                      uint8_t pin_r, uint8_t pin_servo, uint8_t pin_buzz);

void signal_gate_set_aspect(SignalGateController_t *ctrl, SignalAspect_t aspect);

void signal_gate_set_barrier(SignalGateController_t *ctrl, uint8_t angle_deg);

#ifdef __cplusplus
}
#endif

#endif // SIGNAL_GATE_CONTROLLER_H
