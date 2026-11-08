/**
 * @file config.h
 * @brief Hardware pinout, thresholds, and operational parameters
 *        for the Autonomous Fire Fighting Robot.
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
#define PIN_MOTOR_ENA          5   // PWM Left Motor Speed
#define PIN_MOTOR_IN1          6   // Left Motor Direction 1
#define PIN_MOTOR_IN2          7   // Left Motor Direction 2
#define PIN_MOTOR_IN3          8   // Right Motor Direction 1
#define PIN_MOTOR_IN4          9   // Right Motor Direction 2
#define PIN_MOTOR_ENB          10  // PWM Right Motor Speed

// 3-Channel Infrared Flame Sensor Array (Digital Active-LOW or Analog)
#define PIN_FLAME_LEFT         2   // Left Flame Sensor Digital Pin
#define PIN_FLAME_CENTER       3   // Center Flame Sensor Digital Pin
#define PIN_FLAME_RIGHT        4   // Right Flame Sensor Digital Pin

// Water Extinguisher Subsystem
#define PIN_PUMP_RELAY         11  // Relay Control Pin for Submersible DC Pump
#define PIN_NOZZLE_SERVO       12  // Optional Sweeping Water Nozzle Servo
#define PIN_FIRE_ALARM_BUZZER  13  // Onboard Fire Alert Buzzer / Strobe LED

// ============================================================================
// SYSTEM THRESHOLDS & TIMING CONSTANTS
// ============================================================================

// Motor Drive Speeds (PWM: 0 - 255)
#define SPEED_PATROL           160 // Search patrol cruise speed
#define SPEED_APPROACH         140 // Slower precision approach speed toward fire
#define SPEED_PIVOT_ALIGN      150 // Alignment pivot speed
#define SPEED_HALT             0   // Motors stopped

// Extinguishing Parameters (Milliseconds)
#define EXTINGUISH_SPRAY_MS    3000 // 3.0 second water spray burst
#define REVERIFY_SETTLE_MS     1500 // 1.5 second wait for smoke to clear before re-check
#define MAX_CONSECUTIVE_SPRAYS 3    // Safety limit before raising fault to prevent flooding
#define PUMP_WATCHDOG_MAX_MS   5000 // Hardware cutoff to prevent pump motor burnout

#endif // CONFIG_H
