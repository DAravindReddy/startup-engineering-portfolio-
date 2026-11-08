#ifndef IRRIGATION_SYSTEM_H
#define IRRIGATION_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    IRRIGATION_MODE_MANUAL = 0,
    IRRIGATION_MODE_AUTO = 1
} irrigation_mode_t;

typedef enum {
    PUMP_STATE_IDLE = 0,
    PUMP_STATE_WATERING = 1,
    PUMP_STATE_COOLDOWN = 2,
    PUMP_STATE_SAFETY_TRIP = 3
} pump_state_t;

typedef struct {
    irrigation_mode_t mode;
    pump_state_t state;
    uint8_t dry_threshold_pct;
    uint8_t wet_threshold_pct;
    uint32_t pump_start_time_ms;
    uint32_t pump_stop_time_ms;
    uint32_t max_run_time_ms;
    uint32_t min_rest_time_ms;
    bool pump_active;
    bool emergency_tripped;
} irrigation_controller_t;

#ifdef __cplusplus
extern "C" {
#endif

void irrigation_init(irrigation_controller_t *irr, uint8_t dry_th, uint8_t wet_th, uint32_t max_run_ms, uint32_t min_rest_ms);
void irrigation_set_mode(irrigation_controller_t *irr, irrigation_mode_t mode);
void irrigation_set_thresholds(irrigation_controller_t *irr, uint8_t dry_th, uint8_t wet_th);
bool irrigation_manual_start(irrigation_controller_t *irr, uint32_t current_time_ms);
void irrigation_manual_stop(irrigation_controller_t *irr, uint32_t current_time_ms);

/**
 * @brief Non-blocking evaluation step for closed-loop irrigation.
 * Call periodically inside loop().
 *
 * @param irr Pointer to irrigation controller
 * @param current_moisture_pct Current calibrated soil moisture %
 * @param current_time_ms Current millis() timestamp
 * @return true if pump should be energised, false if de-energised
 */
bool irrigation_update(irrigation_controller_t *irr, uint8_t current_moisture_pct, uint32_t current_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* IRRIGATION_SYSTEM_H */
