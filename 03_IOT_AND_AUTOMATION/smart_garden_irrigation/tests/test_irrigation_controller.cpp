/**
 * @file test_irrigation_controller.cpp
 * @brief Formal unit test harness for Smart Garden Irrigation Controller Engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "irrigation_controller.h"
#include <iostream>
#include <cassert>
#include <cmath>

static void test_initialization() {
    irrigation_controller_t ctrl;
    irrigation_init(&ctrl);

    assert(ctrl.mode == IRRIGATION_MODE_AUTO);
    assert(ctrl.pump_state == PUMP_STATE_IDLE);
    assert(ctrl.pump_relay_active == false);
    assert(ctrl.threshold_low == DEFAULT_MOISTURE_LOW_PCT);
    assert(ctrl.threshold_high == DEFAULT_MOISTURE_HIGH_PCT);
    std::cout << "[PASS] test_initialization\n";
}

static void test_sensor_ema_filtering() {
    irrigation_controller_t ctrl;
    irrigation_init(&ctrl);

    // Initial baseline is 50.0%
    // Inject step reading of 20.0%
    // Next value should be 0.25 * 20 + 0.75 * 50 = 5.0 + 37.5 = 42.5%
    irrigation_update_sensors(&ctrl, 20.0f, 25.0f, 50.0f);
    assert(std::fabs(ctrl.telemetry.filtered_moisture_pct - 42.5f) < 0.01f);

    // Inject next step reading of 20.0%
    // Next value should be 0.25 * 20 + 0.75 * 42.5 = 5.0 + 31.875 = 36.875%
    irrigation_update_sensors(&ctrl, 20.0f, 25.0f, 50.0f);
    assert(std::fabs(ctrl.telemetry.filtered_moisture_pct - 36.875f) < 0.01f);
    std::cout << "[PASS] test_sensor_ema_filtering\n";
}

static void test_environmental_adaptation() {
    irrigation_controller_t ctrl;
    irrigation_init(&ctrl);

    // At 25C and 50% humidity, thresholds match defaults
    irrigation_update_sensors(&ctrl, 50.0f, 25.0f, 50.0f);
    irrigation_adapt_thresholds(&ctrl);
    assert(ctrl.threshold_low == DEFAULT_MOISTURE_LOW_PCT);

    // At 35C (+10C delta -> +4.0% boost) and 30% humidity (+20% deficit -> +3.0% boost)
    // Total boost = 7.0% -> threshold_low = 35 + 7 = 42.0%
    irrigation_update_sensors(&ctrl, 50.0f, 35.0f, 30.0f);
    irrigation_adapt_thresholds(&ctrl);
    assert(std::fabs(ctrl.threshold_low - 42.0f) < 0.01f);
    assert(std::fabs(ctrl.threshold_high - 72.0f) < 0.01f);
    std::cout << "[PASS] test_environmental_adaptation\n";
}

static void test_hysteresis_and_cooldown_cycle() {
    irrigation_controller_t ctrl;
    irrigation_init(&ctrl);

    uint64_t t = 1000ULL;

    // Soil is initially moist (60%)
    ctrl.telemetry.filtered_moisture_pct = 60.0f;
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_IDLE);
    assert(ctrl.pump_relay_active == false);

    // Soil drops below low threshold (30% < 35%)
    ctrl.telemetry.filtered_moisture_pct = 30.0f;
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_IRRIGATING);
    assert(ctrl.pump_relay_active == true);
    assert(ctrl.total_irrigation_cycles == 1);

    // Soil rises to 50% (between 35% and 65%), pump must STAY ACTIVE (hysteresis)
    t += 5000ULL;
    ctrl.telemetry.filtered_moisture_pct = 50.0f;
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_IRRIGATING);
    assert(ctrl.pump_relay_active == true);

    // Soil reaches 66% (>= 65%), pump stops and enters cooldown
    t += 2000ULL;
    ctrl.telemetry.filtered_moisture_pct = 66.0f;
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_COOLDOWN);
    assert(ctrl.pump_relay_active == false);

    // During cooldown, even if soil immediately drops back to 20%, pump MUST NOT turn on
    t += 4000ULL; // Cooldown is 10s, only 4s elapsed
    ctrl.telemetry.filtered_moisture_pct = 20.0f;
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_COOLDOWN);
    assert(ctrl.pump_relay_active == false);

    // After 10s cooldown expires, enters IDLE and re-triggers
    t += 7000ULL; // 11s elapsed
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_IDLE);
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_IRRIGATING);
    assert(ctrl.pump_relay_active == true);
    std::cout << "[PASS] test_hysteresis_and_cooldown_cycle\n";
}

static void test_max_duration_and_safety_lockout() {
    irrigation_controller_t ctrl;
    irrigation_init(&ctrl);

    uint64_t t = 1000ULL;
    ctrl.telemetry.filtered_moisture_pct = 20.0f;
    irrigation_fsm_tick(&ctrl, t);
    assert(ctrl.pump_state == PUMP_STATE_IRRIGATING);

    // Run for > 15 seconds without moisture rising (e.g. pipe disconnect or sensor stuck)
    t += 16000ULL;
    irrigation_fsm_tick(&ctrl, t);
    // Must shut off due to max duration timeout!
    assert(ctrl.pump_state == PUMP_STATE_COOLDOWN);
    assert(ctrl.pump_relay_active == false);

    // Sensor fault injection
    ctrl.telemetry.sensor_fault = true;
    irrigation_fsm_tick(&ctrl, t + 15000ULL);
    assert(ctrl.pump_state == PUMP_STATE_FAULT_LOCKOUT);
    assert(ctrl.pump_relay_active == false);
    std::cout << "[PASS] test_max_duration_and_safety_lockout\n";
}

int main() {
    std::cout << "========================================================\n";
    std::cout << " Kernelwise Labs: Smart Garden Irrigation Unit Tests    \n";
    std::cout << "========================================================\n";

    test_initialization();
    test_sensor_ema_filtering();
    test_environmental_adaptation();
    test_hysteresis_and_cooldown_cycle();
    test_max_duration_and_safety_lockout();

    std::cout << "========================================================\n";
    std::cout << " ALL UNIT TESTS COMPLETED SUCCESSFULLY (100%)           \n";
    std::cout << "========================================================\n";
    return 0;
}
