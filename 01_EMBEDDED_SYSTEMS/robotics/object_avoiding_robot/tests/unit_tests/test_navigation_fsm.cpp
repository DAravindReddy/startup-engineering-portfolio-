/**
 * @file test_navigation_fsm.cpp
 * @brief Unit tests for Autonomous Object Avoiding Robot Navigation FSM.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <cassert>
#include <cstdio>
#include <cstdint>

#include "../../source_code/include/config.h"
#include "../../source_code/include/motor_driver.h"
#include "../../source_code/include/servo_scanner.h"
#include "../../source_code/include/ultrasonic_sensor.h"
#include "../../source_code/include/navigation_fsm.h"

// Test harness mocks
static void test_cruise_state_nominal() {
    printf("[TEST 1] Verifying nominal cruise state...\n");
    MotorDriver_t motors;
    UltrasonicSensor_t sonar;
    ServoScanner_t scanner;
    NavigationFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    ultrasonic_init(&sonar, 11, 12);
    servo_scanner_init(&scanner, 3);
    nav_fsm_init(&fsm, &motors, &sonar, &scanner);

    // Initial state must be cruise
    assert(nav_fsm_get_state(&fsm) == NAV_STATE_CRUISE);
    assert(motors.current_direction == DRIVE_FORWARD);
    assert(motors.current_speed == SPEED_CRUISE);

    // Mock clear distance (80 cm)
    sonar.last_distance_cm = 80;
    nav_fsm_update(&fsm, 100);

    // Should remain in cruise
    assert(nav_fsm_get_state(&fsm) == NAV_STATE_CRUISE);
    assert(motors.current_direction == DRIVE_FORWARD);
    printf("  --> PASS: Nominal forward cruise maintained.\n");
}

static void test_warning_halts_and_starts_scan() {
    printf("[TEST 2] Verifying warning distance triggers halt and left scan...\n");
    MotorDriver_t motors;
    UltrasonicSensor_t sonar;
    ServoScanner_t scanner;
    NavigationFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    ultrasonic_init(&sonar, 11, 12);
    servo_scanner_init(&scanner, 3);
    nav_fsm_init(&fsm, &motors, &sonar, &scanner);

    // Obstacle detected at 25 cm (below warning threshold 30 cm)
    fsm.sonar = nullptr; // Decouple hardware ping for unit test
    fsm.dist_forward = 25;
    // Direct state injection to test logic
    nav_fsm_update(&fsm, 100);

    // Must halt motors
    assert(nav_fsm_get_state(&fsm) == NAV_STATE_HALT);
    assert(motors.current_direction == DRIVE_STOP);

    // Next tick moves into scan left
    nav_fsm_update(&fsm, 120);
    assert(nav_fsm_get_state(&fsm) == NAV_STATE_SCAN_LEFT);
    assert(scanner.current_angle == SERVO_ANGLE_LEFT);
    printf("  --> PASS: Motors halted and left scan initiated at %d degrees.\n", SERVO_ANGLE_LEFT);
}

static void test_path_decision_left_clear() {
    printf("[TEST 3] Verifying path decision chooses clearer left path...\n");
    MotorDriver_t motors;
    UltrasonicSensor_t sonar;
    ServoScanner_t scanner;
    NavigationFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    ultrasonic_init(&sonar, 11, 12);
    servo_scanner_init(&scanner, 3);
    nav_fsm_init(&fsm, &motors, &sonar, &scanner);

    // Set FSM in DECIDE_PATH state with Left: 70cm, Right: 20cm
    fsm.sonar = nullptr;
    fsm.state = NAV_STATE_DECIDE_PATH;
    fsm.dist_left = 70;
    fsm.dist_right = 20;

    nav_fsm_update(&fsm, 1000);

    assert(nav_fsm_get_state(&fsm) == NAV_STATE_PIVOT_LEFT);
    assert(motors.current_direction == DRIVE_PIVOT_LEFT);
    assert(motors.current_speed == SPEED_SCAN_TURN);
    printf("  --> PASS: Differential left pivot triggered when left clearance > right clearance.\n");
}

static void test_emergency_critical_reverse() {
    printf("[TEST 4] Verifying emergency reverse upon critical proximity...\n");
    MotorDriver_t motors;
    UltrasonicSensor_t sonar;
    ServoScanner_t scanner;
    NavigationFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    ultrasonic_init(&sonar, 11, 12);
    servo_scanner_init(&scanner, 3);
    nav_fsm_init(&fsm, &motors, &sonar, &scanner);

    // Critical proximity: 10 cm (below DISTANCE_CRITICAL_CM 15cm)
    fsm.sonar = nullptr;
    fsm.dist_forward = 10;
    nav_fsm_update(&fsm, 500);

    assert(nav_fsm_get_state(&fsm) == NAV_STATE_REVERSE_BACKUP);
    assert(motors.current_direction == DRIVE_REVERSE);
    assert(motors.current_speed == SPEED_CREEP);
    printf("  --> PASS: Immediate emergency reverse triggered at %d cm.\n", fsm.dist_forward);
}

int main() {
    printf("====================================================\n");
    printf(" Autonomous Object Avoiding Robot FSM Unit Tests    \n");
    printf("====================================================\n");

    test_cruise_state_nominal();
    test_warning_halts_and_starts_scan();
    test_path_decision_left_clear();
    test_emergency_critical_reverse();

    printf("\n>>> ALL 4 TEST SUITES PASSED CLEANLY (100%% SUCCESS) <<<\n");
    return 0;
}
