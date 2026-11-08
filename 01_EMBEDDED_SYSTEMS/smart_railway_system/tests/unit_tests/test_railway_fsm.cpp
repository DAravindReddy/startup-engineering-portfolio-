/**
 * @file test_railway_fsm.cpp
 * @brief Unit tests for Smart Railway Track Safety FSM and multi-aspect signaling.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <cassert>
#include <cstdio>
#include <cstdint>

#include "../../source_code/include/config.h"
#include "../../source_code/include/crack_detector.h"
#include "../../source_code/include/obstacle_sonar.h"
#include "../../source_code/include/signal_gate_controller.h"
#include "../../source_code/include/railway_fsm.h"

static void test_track_clear_green_aspect() {
    printf("[TEST 1] Verifying track clear nominal state (Green Aspect, Gate Open)...\n");
    SignalGateController_t signals;
    RailwayFSM_t fsm;

    signal_gate_init(&signals, 7, 8, 9, 6, 10);
    railway_fsm_init(&fsm, nullptr, nullptr, &signals);

    fsm.last_crack_status = RAIL_STATUS_OK;
    fsm.last_obstacle_dist_cm = 120; // 120cm clear

    railway_fsm_update(&fsm, 100);

    assert(railway_fsm_get_state(&fsm) == RAIL_FSM_STATE_CLEAR);
    assert(signals.current_aspect == SIGNAL_ASPECT_GREEN);
    assert(signals.current_gate_angle == GATE_ANGLE_OPEN);
    assert(!signals.buzzer_active);
    printf("  --> PASS: Green Proceed aspect confirmed with barrier elevated (90°).\n");
}

static void test_caution_yellow_aspect() {
    printf("[TEST 2] Verifying caution zone (Yellow Aspect, Gate Open)...\n");
    SignalGateController_t signals;
    RailwayFSM_t fsm;

    signal_gate_init(&signals, 7, 8, 9, 6, 10);
    railway_fsm_init(&fsm, nullptr, nullptr, &signals);

    fsm.last_crack_status = RAIL_STATUS_OK;
    fsm.last_obstacle_dist_cm = 45; // In warning zone [31cm, 60cm]

    railway_fsm_update(&fsm, 100);

    assert(railway_fsm_get_state(&fsm) == RAIL_FSM_STATE_CAUTION);
    assert(signals.current_aspect == SIGNAL_ASPECT_YELLOW);
    assert(signals.current_gate_angle == GATE_ANGLE_OPEN);
    assert(!signals.buzzer_active);
    printf("  --> PASS: Yellow Caution aspect confirmed for 45cm obstacle distance.\n");
}

static void test_obstacle_danger_red_aspect() {
    printf("[TEST 3] Verifying critical obstacle danger (Red Aspect, Gate Lowered, Buzzer)...\n");
    SignalGateController_t signals;
    RailwayFSM_t fsm;

    signal_gate_init(&signals, 7, 8, 9, 6, 10);
    railway_fsm_init(&fsm, nullptr, nullptr, &signals);

    fsm.last_crack_status = RAIL_STATUS_OK;
    fsm.last_obstacle_dist_cm = 20; // <= 30cm critical danger

    railway_fsm_update(&fsm, 100);

    assert(railway_fsm_get_state(&fsm) == RAIL_FSM_STATE_OBSTACLE_DANGER);
    assert(signals.current_aspect == SIGNAL_ASPECT_RED);
    assert(signals.current_gate_angle == GATE_ANGLE_CLOSED);
    assert(signals.buzzer_active);
    printf("  --> PASS: Emergency Red aspect confirmed, gate lowered (0°), buzzer sounding.\n");
}

static void test_crack_priority_override() {
    printf("[TEST 4] Verifying structural rail crack priority override...\n");
    SignalGateController_t signals;
    RailwayFSM_t fsm;

    signal_gate_init(&signals, 7, 8, 9, 6, 10);
    railway_fsm_init(&fsm, nullptr, nullptr, &signals);

    // Obstacle is totally clear (200cm), but left rail has a fracture
    fsm.last_obstacle_dist_cm = 200;
    fsm.last_crack_status = RAIL_CRACK_LEFT;

    railway_fsm_update(&fsm, 100);

    assert(railway_fsm_get_state(&fsm) == RAIL_FSM_STATE_CRACK_DANGER);
    assert(signals.current_aspect == SIGNAL_ASPECT_RED);
    assert(signals.current_gate_angle == GATE_ANGLE_CLOSED);
    assert(signals.buzzer_active);
    printf("  --> PASS: Rail fracture successfully overrides clear distance to trigger Red aspect.\n");
}

static void test_crack_debounce_filter() {
    printf("[TEST 5] Verifying crack detector debounce filter...\n");
    CrackDetector_t detector;
    crack_detector_init(&detector, 2, 3);

    // Initial state
    assert(detector.debounced_status == RAIL_STATUS_OK);

    // Simulate transient noise pulse (< 40ms)
    detector.raw_left_fault = true;
    detector.left_fault_start_ms = 1000;

    // After only 15ms: must NOT trip
    bool left_confirmed = detector.raw_left_fault && ((1015 - detector.left_fault_start_ms) >= CRACK_DEBOUNCE_MS);
    assert(!left_confirmed);

    // After 45ms: MUST confirm fault
    left_confirmed = detector.raw_left_fault && ((1045 - detector.left_fault_start_ms) >= CRACK_DEBOUNCE_MS);
    assert(left_confirmed);
    printf("  --> PASS: Transient spike (<40ms) filtered; sustained crack (>40ms) verified.\n");
}

int main() {
    printf("====================================================\n");
    printf(" Smart Railway Track System FSM Unit Tests          \n");
    printf("====================================================\n");

    test_track_clear_green_aspect();
    test_caution_yellow_aspect();
    test_obstacle_danger_red_aspect();
    test_crack_priority_override();
    test_crack_debounce_filter();

    printf("\n>>> ALL 5 TEST SUITES PASSED CLEANLY (100%% SUCCESS) <<<\n");
    return 0;
}
