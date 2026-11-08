/**
 * @file config.h
 * @brief System configuration, BLE UUIDs, sampling rates, and TinyML thresholds
 *        for Arduino Nano 33 BLE Sense Machine Health AI.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// BLE SERVICE & CHARACTERISTIC UUID DEFINITIONS
// ============================================================================

// Custom 128-bit UUID for Kernelwise Machine Health Monitoring Service
#define BLE_UUID_HEALTH_SERVICE          "19B10000-E8F2-537E-4F6C-D104768A1214"
#define BLE_UUID_CHAR_HEALTH_SCORE       "19B10001-E8F2-537E-4F6C-D104768A1214" // Anomaly Score (0-100%)
#define BLE_UUID_CHAR_VIBRATION_RMS      "19B10002-E8F2-537E-4F6C-D104768A1214" // Vibration RMS (m/s²)
#define BLE_UUID_CHAR_TEMPERATURE        "19B10003-E8F2-537E-4F6C-D104768A1214" // Temperature (°C)
#define BLE_UUID_CHAR_MACHINE_STATE      "19B10004-E8F2-537E-4F6C-D104768A1214" // Normal / Warning / Fault

#define BLE_DEVICE_NAME                  "KW-EdgeHealth-01"

// ============================================================================
// SAMPLING & FEATURE EXTRACTION PARAMETERS
// ============================================================================

#define IMU_SAMPLE_RATE_HZ               100 // 100 Hz accelerometer sampling
#define VIBRATION_WINDOW_SIZE            64  // 64-sample rolling window for RMS & Kurtosis
#define ENV_SAMPLE_INTERVAL_MS           1000 // 1 Hz environmental poll (Temp/Humidity)
#define INFERENCE_INTERVAL_MS            500  // 2 Hz TinyML anomaly inference loop

// Machine Fault Thresholds (Vibration in m/s², Temp in °C)
#define VIBRATION_RMS_NORMAL_MAX         4.5f  // Normal smooth running motor
#define VIBRATION_RMS_WARNING_MAX        9.0f  // Bearing degradation warning
#define TEMP_NORMAL_MAX_C                55.0f // Normal thermal operating range
#define TEMP_CRITICAL_MAX_C              80.0f // Severe thermal overheating

// Anomaly Score Thresholds (0.0% to 100.0%)
#define ANOMALY_SCORE_WARNING_PCT        50.0f
#define ANOMALY_SCORE_CRITICAL_PCT       75.0f

#endif // CONFIG_H
