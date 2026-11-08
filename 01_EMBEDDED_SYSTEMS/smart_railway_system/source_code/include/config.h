/**
 * @file config.h
 * @brief Pin definitions, thresholds, and operational parameters
 *        for the Smart Railway Track Inspection and Safety Signaling System.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// HARDWARE PIN DEFINITIONS (Arduino UNO Platform)
// ============================================================================

// Dual-Rail Infrared Crack Sensors (Active-LOW on break/crack)
#define PIN_CRACK_LEFT_RAIL    2   // Left Rail IR Reflective Sensor (Digital Pin)
#define PIN_CRACK_RIGHT_RAIL   3   // Right Rail IR Reflective Sensor (Digital Pin)

// HC-SR04 Ultrasonic Sonar for Track Obstacle Detection
#define PIN_SONAR_TRIG         4   // Ultrasonic Trigger Output
#define PIN_SONAR_ECHO         5   // Ultrasonic Echo Input

// Automated Level-Crossing Gate Servo
#define PIN_GATE_SERVO         6   // PWM Control for Crossing Barrier Gate

// Railway Traffic Signal Aspects (Color Light Signals)
#define PIN_SIGNAL_GREEN       7   // Track Clear (Proceed Aspect)
#define PIN_SIGNAL_YELLOW      8   // Caution / Track Warning Aspect
#define PIN_SIGNAL_RED         9   // Track Danger / Emergency Halt Aspect

// Acoustic Alarm & Notification
#define PIN_ALARM_BUZZER       10  // Emergency Strobe & Alert Siren

// Inspection Vehicle Drive Motor (Optional Autonomous Trolley Mode)
#define PIN_TROLLEY_MOTOR_ENA  11  // PWM Speed Control for Inspection Trolley
#define PIN_TROLLEY_MOTOR_IN1  12  // Direction Pin 1
#define PIN_TROLLEY_MOTOR_IN2  13  // Direction Pin 2

// ============================================================================
// SYSTEM THRESHOLDS & TIMING CONSTANTS
// ============================================================================

// Sonar Distance Thresholds (Centimeters)
#define TRACK_OBSTACLE_DANGER_CM  30  // Critical obstacle on tracks -> Emergency Red Signal
#define TRACK_OBSTACLE_WARNING_CM 60  // Approaching obstacle -> Yellow Caution Signal
#define TRACK_CLEAR_MIN_CM        100 // Clean track clearance

// Gate Barrier Servo Angles (Degrees: 0 - 90)
#define GATE_ANGLE_OPEN           90  // Barrier Elevated (Road Open, No Train)
#define GATE_ANGLE_CLOSED         0   // Barrier Lowered (Road Closed, Danger)

// Trolley Speed Settings (PWM: 0 - 255)
#define SPEED_INSPECTION_CRUISE   160 // Steady track scanning speed
#define SPEED_EMERGENCY_HALT      0   // Trolley stopped

// Fault Debounce & Safety Timing
#define CRACK_DEBOUNCE_MS         40  // Minimum crack pulse width to filter rail vibration
#define SAMPLING_PERIOD_MS        50  // 20 Hz FSM update period

#endif // CONFIG_H
