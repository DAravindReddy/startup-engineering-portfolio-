/**
 * @file servo_controller.h
 * @brief 4-DOF Joint Servo Controller with slew-rate smoothing.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <stdint.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t pin_base;
    uint8_t pin_shoulder;
    uint8_t pin_elbow;
    uint8_t pin_gripper;

    // Target Joint Angles (Degrees: 0 - 180)
    float target_base;
    float target_shoulder;
    float target_elbow;
    float target_gripper;

    // Current Smoothed Joint Angles
    float current_base;
    float current_shoulder;
    float current_elbow;
    float current_gripper;

    uint32_t last_update_ms;
} ServoController_t;

void servo_controller_init(ServoController_t *controller, uint8_t pin_b, uint8_t pin_s,
                           uint8_t pin_e, uint8_t pin_g);

void servo_controller_set_targets(ServoController_t *controller, float base_deg,
                                 float shoulder_deg, float elbow_deg, float gripper_deg);

void servo_controller_update(ServoController_t *controller, uint32_t current_time_ms);

#ifdef __cplusplus
}
#endif

#endif // SERVO_CONTROLLER_H
