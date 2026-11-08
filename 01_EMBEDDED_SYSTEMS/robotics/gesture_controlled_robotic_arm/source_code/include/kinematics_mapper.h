/**
 * @file kinematics_mapper.h
 * @brief Gesture-to-joint angle kinematic translation and coordinate mapping.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef KINEMATICS_MAPPER_H
#define KINEMATICS_MAPPER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float hand_roll_deg;
    float hand_pitch_deg;
    bool gripper_trigger_active;

    // Computed Target Joint Angles
    float joint_base_deg;
    float joint_shoulder_deg;
    float joint_elbow_deg;
    float joint_gripper_deg;
} KinematicsMapper_t;

void kinematics_init(KinematicsMapper_t *mapper);

void kinematics_map_gesture(KinematicsMapper_t *mapper, float roll_deg,
                            float pitch_deg, bool grip_pressed);

#ifdef __cplusplus
}
#endif

#endif // KINEMATICS_MAPPER_H
