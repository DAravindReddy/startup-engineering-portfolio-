/**
 * @file kinematics_mapper.cpp
 * @brief Gesture-to-joint angle kinematic translation implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "kinematics_mapper.h"

static float map_range(float val, float in_min, float in_max, float out_min, float out_max) {
    if (val < in_min) val = in_min;
    if (val > in_max) val = in_max;
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void kinematics_init(KinematicsMapper_t *mapper) {
    if (!mapper) return;

    mapper->hand_roll_deg = 0.0f;
    mapper->hand_pitch_deg = 0.0f;
    mapper->gripper_trigger_active = false;

    mapper->joint_base_deg = BASE_REST_ANGLE;
    mapper->joint_shoulder_deg = SHOULDER_REST_ANGLE;
    mapper->joint_elbow_deg = ELBOW_REST_ANGLE;
    mapper->joint_gripper_deg = GRIPPER_REST_ANGLE;
}

void kinematics_map_gesture(KinematicsMapper_t *mapper, float roll_deg,
                            float pitch_deg, bool grip_pressed) {
    if (!mapper) return;

    mapper->hand_roll_deg = roll_deg;
    mapper->hand_pitch_deg = pitch_deg;
    mapper->gripper_trigger_active = grip_pressed;

    // 1. Hand Roll (-50° to +50°) controls Base Joint Pan (10° to 170°)
    // Tilting hand right rotates base clockwise; tilting left rotates counter-clockwise
    mapper->joint_base_deg = map_range(roll_deg, -50.0f, 50.0f, BASE_MIN_ANGLE, BASE_MAX_ANGLE);

    // 2. Hand Pitch (-45° to +45°) controls Shoulder Joint Elevation (30° to 150°)
    // Tilting hand upward raises arm; tilting downward lowers arm
    mapper->joint_shoulder_deg = map_range(pitch_deg, -45.0f, 45.0f, SHOULDER_MIN_ANGLE, SHOULDER_MAX_ANGLE);

    // 3. Elbow Joint Articulation (coupled inverse to shoulder for horizon compensation)
    // As shoulder elevates forward, elbow articulates slightly inward to maintain level end-effector
    float elbow_compensated = 180.0f - mapper->joint_shoulder_deg;
    if (elbow_compensated < ELBOW_MIN_ANGLE) elbow_compensated = ELBOW_MIN_ANGLE;
    if (elbow_compensated > ELBOW_MAX_ANGLE) elbow_compensated = ELBOW_MAX_ANGLE;
    mapper->joint_elbow_deg = elbow_compensated;

    // 4. Gripper Clamp state
    mapper->joint_gripper_deg = grip_pressed ? GRIPPER_CLOSED_ANGLE : GRIPPER_OPEN_ANGLE;
}
