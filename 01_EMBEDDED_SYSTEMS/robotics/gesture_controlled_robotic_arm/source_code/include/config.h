/**
 * @file config.h
 * @brief Configuration, pinouts, kinematic limits, and filter coefficients
 *        for the Gesture Controlled 4-DOF Robotic Arm.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// HARDWARE PIN DEFINITIONS (Arduino UNO / Nano Platform)
// ============================================================================

// I2C Interface for MPU6050 IMU
#define PIN_I2C_SDA            A4  // I2C Data Line (Arduino UNO/Nano)
#define PIN_I2C_SCL            A5  // I2C Clock Line (Arduino UNO/Nano)
#define MPU6050_I2C_ADDR       0x68 // Default I2C Slave Address (AD0 low)

// 4-DOF Servo PWM Pins (Timer-backed PWM outputs)
#define PIN_SERVO_BASE         3   // Base Rotation Servo (Pan: Axis 1)
#define PIN_SERVO_SHOULDER     5   // Shoulder Elevation Servo (Pitch: Axis 2)
#define PIN_SERVO_ELBOW        6   // Elbow Articulation Servo (Pitch: Axis 3)
#define PIN_SERVO_GRIPPER      9   // End-Effector Gripper Clamp Servo (Axis 4)

// Auxiliary Inputs
#define PIN_GRIPPER_BUTTON     2   // Pushbutton or Flex Sensor for Clamp Trigger
#define PIN_STATUS_LED         13  // Onboard Heartbeat / Status LED

// ============================================================================
// KINEMATIC SERVO ANGLE LIMITS (Degrees: 0 - 180)
// ============================================================================

// Base Joint (Horizontal Swivel)
#define BASE_MIN_ANGLE         10
#define BASE_MAX_ANGLE         170
#define BASE_REST_ANGLE        90

// Shoulder Joint (Arm Lift)
#define SHOULDER_MIN_ANGLE     20
#define SHOULDER_MAX_ANGLE     160
#define SHOULDER_REST_ANGLE    90

// Elbow Joint (Forearm Reach)
#define ELBOW_MIN_ANGLE        15
#define ELBOW_MAX_ANGLE        165
#define ELBOW_REST_ANGLE       90

// Gripper Claw (End-Effector)
#define GRIPPER_OPEN_ANGLE     40  // Wide Open Jaw
#define GRIPPER_CLOSED_ANGLE   115 // Clamped Object Holding
#define GRIPPER_REST_ANGLE     40

// ============================================================================
// MOTION FILTERING & ACCELERATION LIMITS
// ============================================================================

#define SERVO_SLEW_RATE_DEG_PER_SEC 90 // Max rotational velocity (prevents tooth stripping)
#define IMU_COMPLEMENTARY_ALPHA     0.96 // Gyro weight vs Accelerometer weight
#define SAMPLING_PERIOD_MS          20   // 50 Hz control update loop

#endif // CONFIG_H
