/**
 * @file servo_controller.cpp
 * @brief 4-DOF Joint Servo Controller implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "servo_controller.h"
#include <math.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <Servo.h>
static Servo g_servo_base;
static Servo g_servo_shoulder;
static Servo g_servo_elbow;
static Servo g_servo_gripper;
#endif

static float clamp(float val, float min_val, float max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

static float step_towards(float current, float target, float max_step) {
    if (fabsf(target - current) <= max_step) {
        return target;
    }
    return (target > current) ? (current + max_step) : (current - max_step);
}

void servo_controller_init(ServoController_t *controller, uint8_t pin_b, uint8_t pin_s,
                           uint8_t pin_e, uint8_t pin_g) {
    if (!controller) return;

    controller->pin_base = pin_b;
    controller->pin_shoulder = pin_s;
    controller->pin_elbow = pin_e;
    controller->pin_gripper = pin_g;

    controller->target_base = BASE_REST_ANGLE;
    controller->target_shoulder = SHOULDER_REST_ANGLE;
    controller->target_elbow = ELBOW_REST_ANGLE;
    controller->target_gripper = GRIPPER_REST_ANGLE;

    controller->current_base = BASE_REST_ANGLE;
    controller->current_shoulder = SHOULDER_REST_ANGLE;
    controller->current_elbow = ELBOW_REST_ANGLE;
    controller->current_gripper = GRIPPER_REST_ANGLE;
    controller->last_update_ms = 0;

#ifdef ARDUINO
    g_servo_base.attach(pin_b);
    g_servo_shoulder.attach(pin_s);
    g_servo_elbow.attach(pin_e);
    g_servo_gripper.attach(pin_g);

    g_servo_base.write((int)BASE_REST_ANGLE);
    g_servo_shoulder.write((int)SHOULDER_REST_ANGLE);
    g_servo_elbow.write((int)ELBOW_REST_ANGLE);
    g_servo_gripper.write((int)GRIPPER_REST_ANGLE);
#endif
}

void servo_controller_set_targets(ServoController_t *controller, float base_deg,
                                 float shoulder_deg, float elbow_deg, float gripper_deg) {
    if (!controller) return;

    controller->target_base     = clamp(base_deg, BASE_MIN_ANGLE, BASE_MAX_ANGLE);
    controller->target_shoulder = clamp(shoulder_deg, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
    controller->target_elbow    = clamp(elbow_deg, ELBOW_MIN_ANGLE, ELBOW_MAX_ANGLE);
    controller->target_gripper  = clamp(gripper_deg, GRIPPER_OPEN_ANGLE, GRIPPER_CLOSED_ANGLE);
}

void servo_controller_update(ServoController_t *controller, uint32_t current_time_ms) {
    if (!controller) return;

    float dt = (current_time_ms - controller->last_update_ms) / 1000.0f;
    if (dt <= 0.0f || dt > 0.5f) dt = 0.02f;
    controller->last_update_ms = current_time_ms;

    float max_delta = SERVO_SLEW_RATE_DEG_PER_SEC * dt;

    controller->current_base     = step_towards(controller->current_base, controller->target_base, max_delta);
    controller->current_shoulder = step_towards(controller->current_shoulder, controller->target_shoulder, max_delta);
    controller->current_elbow    = step_towards(controller->current_elbow, controller->target_elbow, max_delta);
    controller->current_gripper  = step_towards(controller->current_gripper, controller->target_gripper, max_delta);

#ifdef ARDUINO
    g_servo_base.write((int)controller->current_base);
    g_servo_shoulder.write((int)controller->current_shoulder);
    g_servo_elbow.write((int)controller->current_elbow);
    g_servo_gripper.write((int)controller->current_gripper);
#endif
}
