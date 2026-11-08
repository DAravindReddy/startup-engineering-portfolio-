/**
 * @file fire_fighting_fsm.cpp
 * @brief Autonomous Fire Fighting Navigation and Extinguishing Finite State Machine implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "fire_fighting_fsm.h"

void ff_fsm_init(FireFightingFSM_t *fsm, MotorDriver_t *motors,
                 FlameSensorArray_t *sensors, PumpController_t *pump) {
    if (!fsm) return;

    fsm->state = FF_STATE_PATROL_SEARCH;
    fsm->state_entry_time_ms = 0;
    fsm->detected_flame_pos = FLAME_NONE;
    fsm->motors = motors;
    fsm->flame_sensors = sensors;
    fsm->pump = pump;

    if (fsm->pump) {
        pump_controller_stop(fsm->pump);
    }
    if (fsm->motors) {
        motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_PATROL);
    }
}

void ff_fsm_update(FireFightingFSM_t *fsm, uint32_t current_time_ms) {
    if (!fsm) return;

    uint32_t elapsed = current_time_ms - fsm->state_entry_time_ms;

    // Sample flame detectors if sensor instance is connected
    if (fsm->flame_sensors) {
        fsm->detected_flame_pos = flame_sensors_sample(fsm->flame_sensors);
    }

    // Maintain pump controller update
    if (fsm->pump) {
        pump_controller_update(fsm->pump, current_time_ms);
    }

    switch (fsm->state) {
        case FF_STATE_PATROL_SEARCH: {
            if (fsm->detected_flame_pos == FLAME_POS_CENTER) {
                // Flame straight ahead: advance towards it
                fsm->state = FF_STATE_APPROACH_FIRE;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_APPROACH);
            } else if (fsm->detected_flame_pos == FLAME_POS_LEFT) {
                // Flame on left: pivot left to center it
                fsm->state = FF_STATE_ALIGN_LEFT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_PIVOT_LEFT, SPEED_PIVOT_ALIGN);
            } else if (fsm->detected_flame_pos == FLAME_POS_RIGHT) {
                // Flame on right: pivot right to center it
                fsm->state = FF_STATE_ALIGN_RIGHT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_PIVOT_RIGHT, SPEED_PIVOT_ALIGN);
            } else {
                // No flame: continue gentle search patrol
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_PATROL);
            }
            break;
        }

        case FF_STATE_ALIGN_LEFT: {
            if (fsm->detected_flame_pos == FLAME_POS_CENTER || fsm->detected_flame_pos == FLAME_POS_MULTIPLE) {
                // Flame is now centered: switch to approach
                fsm->state = FF_STATE_APPROACH_FIRE;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_APPROACH);
            } else if (fsm->detected_flame_pos == FLAME_NONE && elapsed >= 1000) {
                // Lost tracking: revert to patrol
                fsm->state = FF_STATE_PATROL_SEARCH;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_PATROL);
            }
            break;
        }

        case FF_STATE_ALIGN_RIGHT: {
            if (fsm->detected_flame_pos == FLAME_POS_CENTER || fsm->detected_flame_pos == FLAME_POS_MULTIPLE) {
                // Flame is now centered: switch to approach
                fsm->state = FF_STATE_APPROACH_FIRE;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_APPROACH);
            } else if (fsm->detected_flame_pos == FLAME_NONE && elapsed >= 1000) {
                // Lost tracking: revert to patrol
                fsm->state = FF_STATE_PATROL_SEARCH;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_PATROL);
            }
            break;
        }

        case FF_STATE_APPROACH_FIRE: {
            // Once within extinguishing proximity (or center + multiple detectors strongly lit)
            // Halt and begin water suppression
            fsm->state = FF_STATE_EXTINGUISH_SPRAY;
            fsm->state_entry_time_ms = current_time_ms;
            if (fsm->motors) motor_driver_stop(fsm->motors);
            if (fsm->pump) pump_controller_start(fsm->pump, current_time_ms);
            break;
        }

        case FF_STATE_EXTINGUISH_SPRAY: {
            if (elapsed >= EXTINGUISH_SPRAY_MS) {
                // Spray complete: halt pump and enter smoke/settle verification
                if (fsm->pump) pump_controller_stop(fsm->pump);
                fsm->state = FF_STATE_REVERIFY;
                fsm->state_entry_time_ms = current_time_ms;
            }
            break;
        }

        case FF_STATE_REVERIFY: {
            if (elapsed >= REVERIFY_SETTLE_MS) {
                // Re-evaluate flame status
                if (fsm->detected_flame_pos != FLAME_NONE) {
                    // Flame still active!
                    if (fsm->pump && fsm->pump->consecutive_spray_count >= MAX_CONSECUTIVE_SPRAYS) {
                        // Max sprays exceeded: lock into fault halt
                        fsm->state = FF_STATE_FAULT_HALT;
                        fsm->state_entry_time_ms = current_time_ms;
                        if (fsm->motors) motor_driver_stop(fsm->motors);
                        if (fsm->pump) pump_controller_stop(fsm->pump);
                    } else {
                        // Re-trigger extinguishing burst
                        fsm->state = FF_STATE_EXTINGUISH_SPRAY;
                        fsm->state_entry_time_ms = current_time_ms;
                        if (fsm->pump) pump_controller_start(fsm->pump, current_time_ms);
                    }
                } else {
                    // Fire successfully extinguished! Reset counter and return to patrol
                    if (fsm->pump) fsm->pump->consecutive_spray_count = 0;
                    fsm->state = FF_STATE_PATROL_SEARCH;
                    fsm->state_entry_time_ms = current_time_ms;
                    if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_PATROL);
                }
            }
            break;
        }

        case FF_STATE_FAULT_HALT:
        default:
            if (fsm->motors) motor_driver_stop(fsm->motors);
            if (fsm->pump) pump_controller_stop(fsm->pump);
            break;
    }
}

FireFightingState_t ff_fsm_get_state(const FireFightingFSM_t *fsm) {
    return fsm ? fsm->state : FF_STATE_IDLE;
}
