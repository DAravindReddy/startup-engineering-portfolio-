/**
 * @file irrigation_controller.h
 * @brief Autonomous IoT Smart Garden Irrigation Controller Engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#ifndef IRRIGATION_CONTROLLER_H
#define IRRIGATION_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_MOISTURE_LOW_PCT   35.0f
#define DEFAULT_MOISTURE_HIGH_PCT  65.0f
#define MAX_IRRIGATION_TIME_MS     15000ULL
#define MANDATORY_COOLDOWN_MS      10000ULL
#define MANUAL_OVERRIDE_TIMEOUT_MS 30000ULL
#define FILTER_ALPHA               0.25f

typedef enum {
    IRRIGATION_MODE_AUTO = 0,
    IRRIGATION_MODE_MANUAL
} irrigation_mode_t;

typedef enum {
    PUMP_STATE_IDLE = 0,
    PUMP_STATE_IRRIGATING,
    PUMP_STATE_COOLDOWN,
    PUMP_STATE_FAULT_LOCKOUT
} pump_state_t;

typedef struct {
    float raw_moisture_pct;
    float filtered_moisture_pct;
    float ambient_temp_c;
    float ambient_humidity_pct;
    bool sensor_fault;
} garden_telemetry_t;

typedef struct {
    irrigation_mode_t mode;
    pump_state_t pump_state;
    bool pump_relay_active;

    float threshold_low;
    float threshold_high;

    uint64_t pump_started_time_ms;
    uint64_t cooldown_started_time_ms;
    uint64_t manual_override_time_ms;

    uint32_t total_irrigation_cycles;
    uint64_t total_runtime_ms;

    garden_telemetry_t telemetry;
} irrigation_controller_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize irrigation controller state structure.
 * @param ctrl Pointer to controller struct.
 */
void irrigation_init(irrigation_controller_t *ctrl);

/**
 * @brief Ingest raw sensor measurements and apply EMA smoothing.
 * @param ctrl Pointer to controller struct.
 * @param raw_moisture Raw calibrated moisture reading (0.0 to 100.0%).
 * @param temp Ambient temperature in Celsius.
 * @param humidity Ambient relative humidity (0.0 to 100.0%).
 */
void irrigation_update_sensors(irrigation_controller_t *ctrl, float raw_moisture, float temp, float humidity);

/**
 * @brief Dynamically adapt moisture thresholds based on ambient heat and vapor deficit.
 * @param ctrl Pointer to controller struct.
 */
void irrigation_adapt_thresholds(irrigation_controller_t *ctrl);

/**
 * @brief Execute core irrigation FSM step.
 * @param ctrl Pointer to controller struct.
 * @param now_ms Monotonic timestamp in milliseconds.
 */
void irrigation_fsm_tick(irrigation_controller_t *ctrl, uint64_t now_ms);

/**
 * @brief Manually command pump ON or OFF with automatic safety timeout.
 * @param ctrl Pointer to controller struct.
 * @param enable True to activate pump, False to halt.
 * @param now_ms Monotonic timestamp in milliseconds.
 */
void irrigation_manual_override(irrigation_controller_t *ctrl, bool enable, uint64_t now_ms);

/**
 * @brief Set operational mode (AUTO vs MANUAL).
 * @param ctrl Pointer to controller struct.
 * @param mode Target mode.
 */
void irrigation_set_mode(irrigation_controller_t *ctrl, irrigation_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif /* IRRIGATION_CONTROLLER_H */
