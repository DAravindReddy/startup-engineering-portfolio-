/**
 * @file irrigation_controller.c
 * @brief Implementation of autonomous irrigation controller and safety FSM.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "irrigation_controller.h"
#include <string.h>

void irrigation_init(irrigation_controller_t *ctrl) {
    if (!ctrl) return;
    memset(ctrl, 0, sizeof(irrigation_controller_t));
    ctrl->mode = IRRIGATION_MODE_AUTO;
    ctrl->pump_state = PUMP_STATE_IDLE;
    ctrl->pump_relay_active = false;
    ctrl->threshold_low = DEFAULT_MOISTURE_LOW_PCT;
    ctrl->threshold_high = DEFAULT_MOISTURE_HIGH_PCT;
    ctrl->telemetry.filtered_moisture_pct = 50.0f; // Default baseline
    ctrl->telemetry.ambient_temp_c = 25.0f;
    ctrl->telemetry.ambient_humidity_pct = 50.0f;
}

void irrigation_update_sensors(irrigation_controller_t *ctrl, float raw_moisture, float temp, float humidity) {
    if (!ctrl) return;

    if (raw_moisture < 0.0f || raw_moisture > 100.0f) {
        ctrl->telemetry.sensor_fault = true;
        return;
    }
    ctrl->telemetry.sensor_fault = false;
    ctrl->telemetry.raw_moisture_pct = raw_moisture;
    ctrl->telemetry.ambient_temp_c = temp;
    ctrl->telemetry.ambient_humidity_pct = humidity;

    // Exponential Moving Average (EMA)
    ctrl->telemetry.filtered_moisture_pct = (FILTER_ALPHA * raw_moisture) +
                                            ((1.0f - FILTER_ALPHA) * ctrl->telemetry.filtered_moisture_pct);
}

void irrigation_adapt_thresholds(irrigation_controller_t *ctrl) {
    if (!ctrl) return;

    float base_low = DEFAULT_MOISTURE_LOW_PCT;
    float base_high = DEFAULT_MOISTURE_HIGH_PCT;

    // Environmental compensation: High temperature (>30C) or Low humidity (<40%) elevates threshold
    float temp_delta = ctrl->telemetry.ambient_temp_c - 25.0f;
    float humidity_deficit = (50.0f - ctrl->telemetry.ambient_humidity_pct);

    float boost = 0.0f;
    if (temp_delta > 0.0f) {
        boost += temp_delta * 0.4f; // +0.4% per degC above 25C
    }
    if (humidity_deficit > 0.0f) {
        boost += humidity_deficit * 0.15f; // +0.15% per % below 50% humidity
    }

    // Clamp boost to a reasonable range [0, 15%]
    if (boost > 15.0f) boost = 15.0f;
    if (boost < 0.0f) boost = 0.0f;

    ctrl->threshold_low = base_low + boost;
    ctrl->threshold_high = base_high + boost;
}

void irrigation_fsm_tick(irrigation_controller_t *ctrl, uint64_t now_ms) {
    if (!ctrl) return;

    if (ctrl->telemetry.sensor_fault) {
        // Sensor fault: Fail-safe shutdown to prevent flooding
        ctrl->pump_state = PUMP_STATE_FAULT_LOCKOUT;
        ctrl->pump_relay_active = false;
        return;
    }

    if (ctrl->mode == IRRIGATION_MODE_MANUAL) {
        // Manual mode timeout watchdog
        if (ctrl->pump_relay_active && (now_ms - ctrl->manual_override_time_ms >= MANUAL_OVERRIDE_TIMEOUT_MS)) {
            ctrl->pump_relay_active = false;
            ctrl->pump_state = PUMP_STATE_COOLDOWN;
            ctrl->cooldown_started_time_ms = now_ms;
        }
        return;
    }

    // AUTO Mode State Machine
    switch (ctrl->pump_state) {
        case PUMP_STATE_IDLE:
            if (ctrl->telemetry.filtered_moisture_pct < ctrl->threshold_low) {
                // Soil is dry: Activate pump
                ctrl->pump_state = PUMP_STATE_IRRIGATING;
                ctrl->pump_relay_active = true;
                ctrl->pump_started_time_ms = now_ms;
                ctrl->total_irrigation_cycles++;
            }
            break;

        case PUMP_STATE_IRRIGATING:
            // Check for max run duration (prevent flooding / pump burnout)
            if (now_ms - ctrl->pump_started_time_ms >= MAX_IRRIGATION_TIME_MS) {
                ctrl->pump_relay_active = false;
                ctrl->pump_state = PUMP_STATE_COOLDOWN;
                ctrl->cooldown_started_time_ms = now_ms;
                ctrl->total_runtime_ms += (now_ms - ctrl->pump_started_time_ms);
            } else if (ctrl->telemetry.filtered_moisture_pct >= ctrl->threshold_high) {
                // Soil reached upper moisture limit
                ctrl->pump_relay_active = false;
                ctrl->pump_state = PUMP_STATE_COOLDOWN;
                ctrl->cooldown_started_time_ms = now_ms;
                ctrl->total_runtime_ms += (now_ms - ctrl->pump_started_time_ms);
            }
            break;

        case PUMP_STATE_COOLDOWN:
            // Enforce minimum soaking period before re-evaluating
            if (now_ms - ctrl->cooldown_started_time_ms >= MANDATORY_COOLDOWN_MS) {
                ctrl->pump_state = PUMP_STATE_IDLE;
            }
            break;

        case PUMP_STATE_FAULT_LOCKOUT:
            ctrl->pump_relay_active = false;
            if (!ctrl->telemetry.sensor_fault) {
                ctrl->pump_state = PUMP_STATE_IDLE;
            }
            break;
    }
}

void irrigation_manual_override(irrigation_controller_t *ctrl, bool enable, uint64_t now_ms) {
    if (!ctrl) return;
    if (enable) {
        ctrl->mode = IRRIGATION_MODE_MANUAL;
        ctrl->pump_state = PUMP_STATE_IRRIGATING;
        ctrl->pump_relay_active = true;
        ctrl->manual_override_time_ms = now_ms;
        ctrl->total_irrigation_cycles++;
    } else {
        ctrl->pump_relay_active = false;
        ctrl->pump_state = PUMP_STATE_IDLE;
    }
}

void irrigation_set_mode(irrigation_controller_t *ctrl, irrigation_mode_t mode) {
    if (!ctrl) return;
    ctrl->mode = mode;
    if (mode == IRRIGATION_MODE_AUTO && ctrl->pump_relay_active) {
        ctrl->pump_relay_active = false;
        ctrl->pump_state = PUMP_STATE_IDLE;
    }
}
