/**
 * @file config.h
 * @brief Hardware pin assignments, navigation thresholds, and timing parameters
 *        for the Autonomous Object Avoiding Robot.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// HARDWARE PIN DEFINITIONS (Arduino UNO Platform)
// ============================================================================

// L298N Dual H-Bridge Motor Driver
#define PIN_MOTOR_ENA          5   // PWM Speed Control Motor Left
#define PIN_MOTOR_IN1          6   // Left Motor Direction 1
#define PIN_MOTOR_IN2          7   // Left Motor Direction 2
#define PIN_MOTOR_IN3          8   // Right Motor Direction 1
#define PIN_MOTOR_IN4          9   // Right Motor Direction 2
#define PIN_MOTOR_ENB          10  // PWM Speed Control Motor Right

// HC-SR04 Ultrasonic Distance Sensor
#define PIN_ULTRASONIC_TRIG    11  // Trigger Pulse Pin (Output)
#define PIN_ULTRASONIC_ECHO    12  // Echo Pulse Pin (Input)

// SG90 Micro Servo (Ultrasonic Panning Mount)
#define PIN_SERVO_PWM          3   // PWM Control Pin for Servo

// Status & Indicator LEDs
#define PIN_STATUS_LED         13  // Onboard Heartbeat / Status Indicator
#define PIN_BUZZER             4   // Optional Obstacle Alert Buzzer

// ============================================================================
// NAVIGATION & THRESHOLD CONSTANTS
// ============================================================================

// Distance Thresholds (in centimeters)
#define DISTANCE_CRITICAL_CM   15  // Immediate reverse/emergency cutoff distance
#define DISTANCE_WARNING_CM    30  // Decision trigger distance to halt and scan
#define DISTANCE_CLEAR_CM      45  // Minimum clearance to resume forward cruise
#define DISTANCE_MAX_VALID_CM  400 // Upper validity bound for HC-SR04 echo

// Motor Speed Profiles (PWM 0 - 255)
#define SPEED_CRUISE           190 // Nominal forward drive speed (~75% duty)
#define SPEED_SCAN_TURN        170 // Differential pivot speed during turns
#define SPEED_CREEP            130 // Low speed maneuvering
#define SPEED_STOP             0   // Motors halted

// Servo Scanner Angles (Degrees: 0 - 180)
#define SERVO_ANGLE_CENTER     90  // Direct forward heading
#define SERVO_ANGLE_LEFT       160 // Left periphery scan heading
#define SERVO_ANGLE_RIGHT      20  // Right periphery scan heading

// Timing Parameters (Milliseconds)
#define SENSOR_POLL_INTERVAL_MS 50  // 20 Hz non-blocking sonar trigger rate
#define SCAN_SETTLE_DELAY_MS   250 // Mechanical settle duration after servo rotation
#define PIVOT_TURN_DURATION_MS 350 // Open-loop differential pivot duration (approx. 60 deg)
#define BACKUP_DURATION_MS     300 // Reverse retreat duration upon critical proximity

#endif // CONFIG_H
