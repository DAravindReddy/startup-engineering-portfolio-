/**
 * @file navigation_fsm.cpp
 * @brief Autonomous obstacle avoidance Finite State Machine implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "navigation_fsm.h"

void nav_fsm_init(NavigationFSM_t *fsm, MotorDriver_t *motors, UltrasonicSensor_t *sonar,
                  ServoScanner_t *scanner) {
    if (!fsm) return;

    fsm->state = NAV_STATE_CRUISE;
    fsm->state_entry_time_ms = 0;
    fsm->dist_forward = DISTANCE_CLEAR_CM;
    fsm->dist_left = DISTANCE_CLEAR_CM;
    fsm->dist_right = DISTANCE_CLEAR_CM;
    fsm->motors = motors;
    fsm->sonar = sonar;
    fsm->scanner = scanner;

    if (fsm->scanner) {
        servo_scanner_set_angle(fsm->scanner, SERVO_ANGLE_CENTER);
    }
    if (fsm->motors) {
        motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_CRUISE);
    }
}

void nav_fsm_update(NavigationFSM_t *fsm, uint32_t current_time_ms) {
    if (!fsm) return;

    uint32_t elapsed = current_time_ms - fsm->state_entry_time_ms;

    switch (fsm->state) {
        case NAV_STATE_CRUISE: {
            // Sample forward clearance
            if (fsm->sonar) {
                fsm->dist_forward = ultrasonic_ping_blocking(fsm->sonar);
            }

            if (fsm->dist_forward <= DISTANCE_CRITICAL_CM) {
                // Emergency immediate stop and reverse
                fsm->state = NAV_STATE_REVERSE_BACKUP;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_REVERSE, SPEED_CREEP);
            } else if (fsm->dist_forward <= DISTANCE_WARNING_CM) {
                // Warning threshold breached: halt and begin sonar scan
                fsm->state = NAV_STATE_HALT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_stop(fsm->motors);
            } else {
                // Path clear: continue forward cruise
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_CRUISE);
            }
            break;
        }

        case NAV_STATE_HALT: {
            // Immediate halt completed: start scanning left
            fsm->state = NAV_STATE_SCAN_LEFT;
            fsm->state_entry_time_ms = current_time_ms;
            if (fsm->scanner) servo_scanner_set_angle(fsm->scanner, SERVO_ANGLE_LEFT);
            break;
        }

        case NAV_STATE_SCAN_LEFT: {
            // Wait for servo to settle mechanically before sampling
            if (elapsed >= SCAN_SETTLE_DELAY_MS) {
                if (fsm->sonar) {
                    fsm->dist_left = ultrasonic_ping_blocking(fsm->sonar);
                }
                // Transition to scan right
                fsm->state = NAV_STATE_SCAN_RIGHT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->scanner) servo_scanner_set_angle(fsm->scanner, SERVO_ANGLE_RIGHT);
            }
            break;
        }

        case NAV_STATE_SCAN_RIGHT: {
            // Wait for servo to settle mechanically before sampling
            if (elapsed >= SCAN_SETTLE_DELAY_MS) {
                if (fsm->sonar) {
                    fsm->dist_right = ultrasonic_ping_blocking(fsm->sonar);
                }
                // Both angles scanned: center servo and proceed to decision
                if (fsm->scanner) servo_scanner_set_angle(fsm->scanner, SERVO_ANGLE_CENTER);
                fsm->state = NAV_STATE_DECIDE_PATH;
                fsm->state_entry_time_ms = current_time_ms;
            }
            break;
        }

        case NAV_STATE_DECIDE_PATH: {
            // Decision Matrix:
            // 1. If both directions are trapped (< DISTANCE_WARNING_CM), back up
            if (fsm->dist_left < DISTANCE_WARNING_CM && fsm->dist_right < DISTANCE_WARNING_CM) {
                fsm->state = NAV_STATE_REVERSE_BACKUP;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_REVERSE, SPEED_CREEP);
            }
            // 2. Clearer on the left
            else if (fsm->dist_left >= fsm->dist_right) {
                fsm->state = NAV_STATE_PIVOT_LEFT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_PIVOT_LEFT, SPEED_SCAN_TURN);
            }
            // 3. Clearer on the right
            else {
                fsm->state = NAV_STATE_PIVOT_RIGHT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_PIVOT_RIGHT, SPEED_SCAN_TURN);
            }
            break;
        }

        case NAV_STATE_PIVOT_LEFT:
        case NAV_STATE_PIVOT_RIGHT: {
            if (elapsed >= PIVOT_TURN_DURATION_MS) {
                // Pivot complete: resume forward cruise
                fsm->state = NAV_STATE_CRUISE;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_FORWARD, SPEED_CRUISE);
            }
            break;
        }

        case NAV_STATE_REVERSE_BACKUP: {
            if (elapsed >= BACKUP_DURATION_MS) {
                // Reverse complete: pivot away from cul-de-sac
                fsm->state = NAV_STATE_PIVOT_RIGHT;
                fsm->state_entry_time_ms = current_time_ms;
                if (fsm->motors) motor_driver_set_motion(fsm->motors, DRIVE_PIVOT_RIGHT, SPEED_SCAN_TURN);
            }
            break;
        }

        default:
            fsm->state = NAV_STATE_CRUISE;
            fsm->state_entry_time_ms = current_time_ms;
            break;
    }
}

NavigationState_t nav_fsm_get_state(const NavigationFSM_t *fsm) {
    return fsm ? fsm->state : NAV_STATE_IDLE;
}
