/**
 * @file railway_fsm.cpp
 * @brief Smart Railway Track Safety and Inspection Finite State Machine implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "railway_fsm.h"

void railway_fsm_init(RailwayFSM_t *fsm, CrackDetector_t *crack_sensor,
                      ObstacleSonar_t *sonar, SignalGateController_t *signals) {
    if (!fsm) return;

    fsm->state = RAIL_FSM_STATE_CLEAR;
    fsm->state_entry_time_ms = 0;
    fsm->last_crack_status = RAIL_STATUS_OK;
    fsm->last_obstacle_dist_cm = TRACK_CLEAR_MIN_CM;
    fsm->crack_sensor = crack_sensor;
    fsm->sonar = sonar;
    fsm->signals = signals;

    if (fsm->signals) {
        signal_gate_set_aspect(fsm->signals, SIGNAL_ASPECT_GREEN);
    }
}

void railway_fsm_update(RailwayFSM_t *fsm, uint32_t current_time_ms) {
    if (!fsm) return;

    // Sample crack sensor
    if (fsm->crack_sensor) {
        fsm->last_crack_status = crack_detector_update(fsm->crack_sensor, current_time_ms);
    }

    // Sample sonar
    if (fsm->sonar) {
        fsm->last_obstacle_dist_cm = obstacle_sonar_ping(fsm->sonar);
    }

    // 1. Highest Priority: Rail Structural Fracture / Crack
    if (fsm->last_crack_status != RAIL_STATUS_OK) {
        if (fsm->state != RAIL_FSM_STATE_CRACK_DANGER) {
            fsm->state = RAIL_FSM_STATE_CRACK_DANGER;
            fsm->state_entry_time_ms = current_time_ms;
            if (fsm->signals) signal_gate_set_aspect(fsm->signals, SIGNAL_ASPECT_RED);
        }
        return;
    }

    // 2. Second Priority: Critical Obstacle on Tracks (<= 30cm)
    if (fsm->last_obstacle_dist_cm <= TRACK_OBSTACLE_DANGER_CM) {
        if (fsm->state != RAIL_FSM_STATE_OBSTACLE_DANGER) {
            fsm->state = RAIL_FSM_STATE_OBSTACLE_DANGER;
            fsm->state_entry_time_ms = current_time_ms;
            if (fsm->signals) signal_gate_set_aspect(fsm->signals, SIGNAL_ASPECT_RED);
        }
        return;
    }

    // 3. Third Priority: Approaching Obstacle in Caution Zone (31cm - 60cm)
    if (fsm->last_obstacle_dist_cm <= TRACK_OBSTACLE_WARNING_CM) {
        if (fsm->state != RAIL_FSM_STATE_CAUTION) {
            fsm->state = RAIL_FSM_STATE_CAUTION;
            fsm->state_entry_time_ms = current_time_ms;
            if (fsm->signals) signal_gate_set_aspect(fsm->signals, SIGNAL_ASPECT_YELLOW);
        }
        return;
    }

    // 4. Default: Track Clear
    if (fsm->state != RAIL_FSM_STATE_CLEAR) {
        fsm->state = RAIL_FSM_STATE_CLEAR;
        fsm->state_entry_time_ms = current_time_ms;
        if (fsm->signals) signal_gate_set_aspect(fsm->signals, SIGNAL_ASPECT_GREEN);
    }
}

RailwayFSMState_t railway_fsm_get_state(const RailwayFSM_t *fsm) {
    return fsm ? fsm->state : RAIL_FSM_STATE_CLEAR;
}
