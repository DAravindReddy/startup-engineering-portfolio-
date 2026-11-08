/**
 * @file test_fire_fighting_fsm.cpp
 * @brief Unit tests for Autonomous Fire Fighting Robot FSM and safety mechanisms.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <cassert>
#include <cstdio>
#include <cstdint>

#include "../../source_code/include/config.h"
#include "../../source_code/include/motor_driver.h"
#include "../../source_code/include/flame_sensor_array.h"
#include "../../source_code/include/pump_controller.h"
#include "../../source_code/include/fire_fighting_fsm.h"

static void test_patrol_search_nominal() {
    printf("[TEST 1] Verifying nominal patrol search when no flame detected...\n");
    MotorDriver_t motors;
    FlameSensorArray_t sensors;
    PumpController_t pump;
    FireFightingFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    flame_sensors_init(&sensors, 2, 3, 4);
    pump_controller_init(&pump, 11);
    ff_fsm_init(&fsm, &motors, &sensors, &pump);

    // Initial state
    assert(ff_fsm_get_state(&fsm) == FF_STATE_PATROL_SEARCH);
    assert(motors.current_direction == DRIVE_FORWARD);
    assert(!pump_controller_is_running(&pump));

    // Update with no flame
    fsm.flame_sensors = nullptr;
    fsm.detected_flame_pos = FLAME_NONE;
    ff_fsm_update(&fsm, 100);

    assert(ff_fsm_get_state(&fsm) == FF_STATE_PATROL_SEARCH);
    assert(motors.current_direction == DRIVE_FORWARD);
    assert(!pump_controller_is_running(&pump));
    printf("  --> PASS: Forward patrol maintained with pump disabled.\n");
}

static void test_flame_detection_and_alignment() {
    printf("[TEST 2] Verifying left/right flame alignment pivots...\n");
    MotorDriver_t motors;
    PumpController_t pump;
    FireFightingFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    pump_controller_init(&pump, 11);
    ff_fsm_init(&fsm, &motors, nullptr, &pump);

    // 1. Left flame detected
    fsm.detected_flame_pos = FLAME_POS_LEFT;
    ff_fsm_update(&fsm, 200);

    assert(ff_fsm_get_state(&fsm) == FF_STATE_ALIGN_LEFT);
    assert(motors.current_direction == DRIVE_PIVOT_LEFT);

    // 2. Right flame detected from patrol
    fsm.state = FF_STATE_PATROL_SEARCH;
    fsm.detected_flame_pos = FLAME_POS_RIGHT;
    ff_fsm_update(&fsm, 300);

    assert(ff_fsm_get_state(&fsm) == FF_STATE_ALIGN_RIGHT);
    assert(motors.current_direction == DRIVE_PIVOT_RIGHT);
    printf("  --> PASS: Directional pivots correctly triggered toward detected flames.\n");
}

static void test_extinguish_spray_cycle() {
    printf("[TEST 3] Verifying extinguishing spray burst and settle reverification...\n");
    MotorDriver_t motors;
    PumpController_t pump;
    FireFightingFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    pump_controller_init(&pump, 11);
    ff_fsm_init(&fsm, &motors, nullptr, &pump);

    // Center flame detected -> switch to approach then extinguish
    fsm.detected_flame_pos = FLAME_POS_CENTER;
    ff_fsm_update(&fsm, 100);

    assert(ff_fsm_get_state(&fsm) == FF_STATE_APPROACH_FIRE);

    // Next tick transitions to extinguish spray
    ff_fsm_update(&fsm, 150);
    assert(ff_fsm_get_state(&fsm) == FF_STATE_EXTINGUISH_SPRAY);
    assert(motors.current_direction == DRIVE_STOP);
    assert(pump_controller_is_running(&pump));

    // Fast-forward elapsed time by EXTINGUISH_SPRAY_MS (3000ms)
    ff_fsm_update(&fsm, 150 + EXTINGUISH_SPRAY_MS + 10);
    assert(ff_fsm_get_state(&fsm) == FF_STATE_REVERIFY);
    assert(!pump_controller_is_running(&pump));

    // Fast-forward settle time by REVERIFY_SETTLE_MS (1500ms) with flame extinguished
    fsm.detected_flame_pos = FLAME_NONE;
    ff_fsm_update(&fsm, 150 + EXTINGUISH_SPRAY_MS + REVERIFY_SETTLE_MS + 20);

    assert(ff_fsm_get_state(&fsm) == FF_STATE_PATROL_SEARCH);
    assert(motors.current_direction == DRIVE_FORWARD);
    assert(pump.consecutive_spray_count == 0);
    printf("  --> PASS: 3.0s spray burst and 1.5s smoke settle cycle completed successfully.\n");
}

static void test_max_consecutive_sprays_safety_cutoff() {
    printf("[TEST 4] Verifying safety lockout upon exceeding maximum spray bursts...\n");
    MotorDriver_t motors;
    PumpController_t pump;
    FireFightingFSM_t fsm;

    motor_driver_init(&motors, 5, 6, 7, 8, 9, 10);
    pump_controller_init(&pump, 11);
    ff_fsm_init(&fsm, &motors, nullptr, &pump);

    // Simulate 3rd failed spray where fire persists
    fsm.state = FF_STATE_REVERIFY;
    fsm.state_entry_time_ms = 1000;
    pump.consecutive_spray_count = MAX_CONSECUTIVE_SPRAYS;
    fsm.detected_flame_pos = FLAME_POS_CENTER;

    // Advance beyond settle time
    ff_fsm_update(&fsm, 1000 + REVERIFY_SETTLE_MS + 50);

    assert(ff_fsm_get_state(&fsm) == FF_STATE_FAULT_HALT);
    assert(motors.current_direction == DRIVE_STOP);
    assert(!pump_controller_is_running(&pump));
    printf("  --> PASS: Safety lockout triggered after %d consecutive spray cycles.\n", MAX_CONSECUTIVE_SPRAYS);
}

int main() {
    printf("====================================================\n");
    printf(" Autonomous Fire Fighting Robot FSM Unit Tests      \n");
    printf("====================================================\n");

    test_patrol_search_nominal();
    test_flame_detection_and_alignment();
    test_extinguish_spray_cycle();
    test_max_consecutive_sprays_safety_cutoff();

    printf("\n>>> ALL 4 TEST SUITES PASSED CLEANLY (100%% SUCCESS) <<<\n");
    return 0;
}
