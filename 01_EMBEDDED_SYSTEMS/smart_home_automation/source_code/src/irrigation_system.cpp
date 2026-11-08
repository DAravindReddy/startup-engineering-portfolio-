#include "irrigation_system.h"

void irrigation_init(irrigation_controller_t *irr, uint8_t dry_th, uint8_t wet_th, uint32_t max_run_ms, uint32_t min_rest_ms) {
    if (!irr) return;
    irr->mode = IRRIGATION_MODE_AUTO;
    irr->state = PUMP_STATE_IDLE;
    irr->dry_threshold_pct = dry_th;
    irr->wet_threshold_pct = wet_th;
    irr->pump_start_time_ms = 0;
    irr->pump_stop_time_ms = 0;
    irr->max_run_time_ms = max_run_ms;
    irr->min_rest_time_ms = min_rest_ms;
    irr->pump_active = false;
    irr->emergency_tripped = false;
}

void irrigation_set_mode(irrigation_controller_t *irr, irrigation_mode_t mode) {
    if (!irr) return;
    irr->mode = mode;
}

void irrigation_set_thresholds(irrigation_controller_t *irr, uint8_t dry_th, uint8_t wet_th) {
    if (!irr) return;
    irr->dry_threshold_pct = dry_th;
    irr->wet_threshold_pct = wet_th;
}

bool irrigation_manual_start(irrigation_controller_t *irr, uint32_t current_time_ms) {
    if (!irr || irr->emergency_tripped) return false;
    irr->pump_active = true;
    irr->state = PUMP_STATE_WATERING;
    irr->pump_start_time_ms = current_time_ms;
    return true;
}

void irrigation_manual_stop(irrigation_controller_t *irr, uint32_t current_time_ms) {
    if (!irr) return;
    irr->pump_active = false;
    irr->state = PUMP_STATE_IDLE;
    irr->pump_stop_time_ms = current_time_ms;
}

bool irrigation_update(irrigation_controller_t *irr, uint8_t current_moisture_pct, uint32_t current_time_ms) {
    if (!irr) return false;

    /* Safety Check: Max Run Time limit applies to both AUTO and MANUAL modes */
    if (irr->pump_active) {
        if ((current_time_ms - irr->pump_start_time_ms) >= irr->max_run_time_ms) {
            irr->pump_active = false;
            irr->state = PUMP_STATE_SAFETY_TRIP;
            irr->pump_stop_time_ms = current_time_ms;
            irr->emergency_tripped = true;
            return false;
        }
    }

    if (irr->mode == IRRIGATION_MODE_MANUAL) {
        return irr->pump_active;
    }

    /* AUTOMATIC MODE WITH HYSTERESIS */
    switch (irr->state) {
        case PUMP_STATE_IDLE:
            /* Cooldown rest check */
            if (irr->pump_stop_time_ms > 0 && (current_time_ms - irr->pump_stop_time_ms) < irr->min_rest_time_ms) {
                return false;
            }

            /* Trigger watering if soil falls below dry threshold */
            if (current_moisture_pct < irr->dry_threshold_pct) {
                irr->pump_active = true;
                irr->state = PUMP_STATE_WATERING;
                irr->pump_start_time_ms = current_time_ms;
            }
            break;

        case PUMP_STATE_WATERING:
            /* De-energize pump when moisture exceeds wet threshold */
            if (current_moisture_pct >= irr->wet_threshold_pct) {
                irr->pump_active = false;
                irr->state = PUMP_STATE_COOLDOWN;
                irr->pump_stop_time_ms = current_time_ms;
            }
            break;

        case PUMP_STATE_COOLDOWN:
            if ((current_time_ms - irr->pump_stop_time_ms) >= irr->min_rest_time_ms) {
                irr->state = PUMP_STATE_IDLE;
                if (current_moisture_pct < irr->dry_threshold_pct) {
                    irr->pump_active = true;
                    irr->state = PUMP_STATE_WATERING;
                    irr->pump_start_time_ms = current_time_ms;
                }
            }
            break;

        case PUMP_STATE_SAFETY_TRIP:
            /* Remains in safety trip until reset */
            irr->pump_active = false;
            break;
    }

    return irr->pump_active;
}
