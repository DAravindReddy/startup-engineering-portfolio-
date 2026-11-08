/**
 * @file test_kinematics_mapper.cpp
 * @brief Unit tests for Gesture Controlled Robotic Arm Kinematics and Slew Rate Controller.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <cassert>
#include <cstdio>
#include <cmath>

#include "../../source_code/include/config.h"
#include "../../source_code/include/mpu6050_imu.h"
#include "../../source_code/include/servo_controller.h"
#include "../../source_code/include/kinematics_mapper.h"

static void test_neutral_gesture_rest_angles() {
    printf("[TEST 1] Verifying neutral hand gesture maps to rest angles...\n");
    KinematicsMapper_t mapper;
    kinematics_init(&mapper);

    kinematics_map_gesture(&mapper, 0.0f, 0.0f, false);

    assert(fabsf(mapper.joint_base_deg - 90.0f) < 1.0f);
    assert(fabsf(mapper.joint_shoulder_deg - 90.0f) < 1.0f);
    assert(fabsf(mapper.joint_elbow_deg - 90.0f) < 1.0f);
    assert(fabsf(mapper.joint_gripper_deg - (float)GRIPPER_OPEN_ANGLE) < 0.1f);
    printf("  --> PASS: Neutral (0,0) maps to Base 90, Shoulder 90, Gripper Open.\n");
}

static void test_extreme_roll_base_limits() {
    printf("[TEST 2] Verifying extreme hand roll clamps to safe base joint boundaries...\n");
    KinematicsMapper_t mapper;
    kinematics_init(&mapper);

    // Roll right +60 deg (exceeds +50 max)
    kinematics_map_gesture(&mapper, 60.0f, 0.0f, false);
    assert(fabsf(mapper.joint_base_deg - (float)BASE_MAX_ANGLE) < 0.1f);

    // Roll left -60 deg (exceeds -50 min)
    kinematics_map_gesture(&mapper, -60.0f, 0.0f, false);
    assert(fabsf(mapper.joint_base_deg - (float)BASE_MIN_ANGLE) < 0.1f);
    printf("  --> PASS: Base angle strictly clamped within [%d°, %d°].\n", BASE_MIN_ANGLE, BASE_MAX_ANGLE);
}

static void test_pitch_shoulder_elevation() {
    printf("[TEST 3] Verifying pitch maps to shoulder elevation...\n");
    KinematicsMapper_t mapper;
    kinematics_init(&mapper);

    // Pitch up +45 deg
    kinematics_map_gesture(&mapper, 0.0f, 45.0f, false);
    assert(fabsf(mapper.joint_shoulder_deg - (float)SHOULDER_MAX_ANGLE) < 0.1f);

    // Pitch down -45 deg
    kinematics_map_gesture(&mapper, 0.0f, -45.0f, false);
    assert(fabsf(mapper.joint_shoulder_deg - (float)SHOULDER_MIN_ANGLE) < 0.1f);
    printf("  --> PASS: Shoulder elevation correctly traverses [%d°, %d°].\n", SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);
}

static void test_gripper_clamp_toggle() {
    printf("[TEST 4] Verifying gripper clamp actuation...\n");
    KinematicsMapper_t mapper;
    kinematics_init(&mapper);

    kinematics_map_gesture(&mapper, 0.0f, 0.0f, true);
    assert(fabsf(mapper.joint_gripper_deg - (float)GRIPPER_CLOSED_ANGLE) < 0.1f);

    kinematics_map_gesture(&mapper, 0.0f, 0.0f, false);
    assert(fabsf(mapper.joint_gripper_deg - (float)GRIPPER_OPEN_ANGLE) < 0.1f);
    printf("  --> PASS: Gripper toggles cleanly between %d° (Open) and %d° (Clamped).\n",
           GRIPPER_OPEN_ANGLE, GRIPPER_CLOSED_ANGLE);
}

static void test_servo_slew_rate_smoothing() {
    printf("[TEST 5] Verifying servo slew-rate velocity limiter...\n");
    ServoController_t controller;
    servo_controller_init(&controller, 3, 5, 6, 9);

    // Initial base angle is 90°
    assert(fabsf(controller.current_base - 90.0f) < 0.1f);

    // Set aggressive target: 170° (80° step)
    servo_controller_set_targets(&controller, 170.0f, 90.0f, 90.0f, 40.0f);

    // Advance 50 ms (0.05 s). Max delta at 90 deg/s = 4.5 degrees
    servo_controller_update(&controller, 50);

    float delta = controller.current_base - 90.0f;
    assert(delta <= 4.6f);
    assert(controller.current_base > 90.0f);
    printf("  --> PASS: Motion smoothed: 80° step limited to +%.2f° in 50ms.\n", delta);
}

int main() {
    printf("====================================================\n");
    printf(" Gesture Controlled Robotic Arm Unit Tests          \n");
    printf("====================================================\n");

    test_neutral_gesture_rest_angles();
    test_extreme_roll_base_limits();
    test_pitch_shoulder_elevation();
    test_gripper_clamp_toggle();
    test_servo_slew_rate_smoothing();

    printf("\n>>> ALL 5 TEST SUITES PASSED CLEANLY (100%% SUCCESS) <<<\n");
    return 0;
}
