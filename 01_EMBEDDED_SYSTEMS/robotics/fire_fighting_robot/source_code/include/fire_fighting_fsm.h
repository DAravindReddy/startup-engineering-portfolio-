/**
 * @file fire_fighting_fsm.h
 * @brief Autonomous Fire Fighting Navigation and Extinguishing Finite State Machine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef FIRE_FIGHTING_FSM_H
#define FIRE_FIGHTING_FSM_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "motor_driver.h"
#include "flame_sensor_array.h"
#include "pump_controller.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FF_STATE_IDLE = 0,
    FF_STATE_PATROL_SEARCH,
    FF_STATE_ALIGN_LEFT,
    FF_STATE_ALIGN_RIGHT,
    FF_STATE_APPROACH_FIRE,
    FF_STATE_EXTINGUISH_SPRAY,
    FF_STATE_REVERIFY,
    FF_STATE_FAULT_HALT
} FireFightingState_t;

typedef struct {
    FireFightingState_t state;
    uint32_t state_entry_time_ms;
    FlamePosition_t detected_flame_pos;
    MotorDriver_t *motors;
    FlameSensorArray_t *flame_sensors;
    PumpController_t *pump;
} FireFightingFSM_t;

void ff_fsm_init(FireFightingFSM_t *fsm, MotorDriver_t *motors,
                 FlameSensorArray_t *sensors, PumpController_t *pump);

void ff_fsm_update(FireFightingFSM_t *fsm, uint32_t current_time_ms);

FireFightingState_t ff_fsm_get_state(const FireFightingFSM_t *fsm);

#ifdef __cplusplus
}
#endif

#endif // FIRE_FIGHTING_FSM_H
