/**
 * @file navigation_fsm.h
 * @brief Autonomous obstacle avoidance Finite State Machine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef NAVIGATION_FSM_H
#define NAVIGATION_FSM_H

#include <stdint.h>
#include "config.h"
#include "motor_driver.h"
#include "servo_scanner.h"
#include "ultrasonic_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NAV_STATE_IDLE = 0,
    NAV_STATE_CRUISE,
    NAV_STATE_HALT,
    NAV_STATE_SCAN_LEFT,
    NAV_STATE_SCAN_RIGHT,
    NAV_STATE_DECIDE_PATH,
    NAV_STATE_PIVOT_LEFT,
    NAV_STATE_PIVOT_RIGHT,
    NAV_STATE_REVERSE_BACKUP
} NavigationState_t;

typedef struct {
    NavigationState_t state;
    uint32_t state_entry_time_ms;
    uint16_t dist_forward;
    uint16_t dist_left;
    uint16_t dist_right;
    MotorDriver_t *motors;
    UltrasonicSensor_t *sonar;
    ServoScanner_t *scanner;
} NavigationFSM_t;

void nav_fsm_init(NavigationFSM_t *fsm, MotorDriver_t *motors, UltrasonicSensor_t *sonar,
                  ServoScanner_t *scanner);

void nav_fsm_update(NavigationFSM_t *fsm, uint32_t current_time_ms);

NavigationState_t nav_fsm_get_state(const NavigationFSM_t *fsm);

#ifdef __cplusplus
}
#endif

#endif // NAVIGATION_FSM_H
