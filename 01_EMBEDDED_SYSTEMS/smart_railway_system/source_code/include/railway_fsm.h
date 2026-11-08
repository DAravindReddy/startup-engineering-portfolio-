/**
 * @file railway_fsm.h
 * @brief Smart Railway Track Safety and Inspection Finite State Machine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef RAILWAY_FSM_H
#define RAILWAY_FSM_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "crack_detector.h"
#include "obstacle_sonar.h"
#include "signal_gate_controller.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RAIL_FSM_STATE_CLEAR = 0,
    RAIL_FSM_STATE_CAUTION,
    RAIL_FSM_STATE_OBSTACLE_DANGER,
    RAIL_FSM_STATE_CRACK_DANGER
} RailwayFSMState_t;

typedef struct {
    RailwayFSMState_t state;
    uint32_t state_entry_time_ms;
    RailCrackStatus_t last_crack_status;
    uint16_t last_obstacle_dist_cm;
    CrackDetector_t *crack_sensor;
    ObstacleSonar_t *sonar;
    SignalGateController_t *signals;
} RailwayFSM_t;

void railway_fsm_init(RailwayFSM_t *fsm, CrackDetector_t *crack_sensor,
                      ObstacleSonar_t *sonar, SignalGateController_t *signals);

void railway_fsm_update(RailwayFSM_t *fsm, uint32_t current_time_ms);

RailwayFSMState_t railway_fsm_get_state(const RailwayFSM_t *fsm);

#ifdef __cplusplus
}
#endif

#endif // RAILWAY_FSM_H
