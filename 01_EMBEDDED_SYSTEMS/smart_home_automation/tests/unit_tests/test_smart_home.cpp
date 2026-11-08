#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../source_code/include/bluetooth_handler.h"
#include "../../source_code/include/sensor_manager.h"
#include "../../source_code/include/actuator_controller.h"
#include "../../source_code/include/irrigation_system.h"

static int g_tests_passed = 0;

void test_bluetooth_command_parser() {
    printf("[TEST] Running Bluetooth Command Parser Tests...\n");

    bt_parsed_command_t cmd;

    cmd = parse_bluetooth_command("L1_ON\r\n");
    assert(cmd.type == CMD_LED1_ON);

    cmd = parse_bluetooth_command("L2_OFF\n");
    assert(cmd.type == CMD_LED2_OFF);

    cmd = parse_bluetooth_command("M_SPEED:185\r\n");
    assert(cmd.type == CMD_MOTOR_SPEED);
    assert(cmd.param_val == 185);

    cmd = parse_bluetooth_command("SERVO:120\n");
    assert(cmd.type == CMD_SERVO_ANGLE);
    assert(cmd.param_val == 120);

    cmd = parse_bluetooth_command("SET_TH:42\n");
    assert(cmd.type == CMD_SET_THRESHOLD);
    assert(cmd.param_val == 42);

    cmd = parse_bluetooth_command("STATUS?\n");
    assert(cmd.type == CMD_GET_STATUS);

    cmd = parse_bluetooth_command("INVALID_COMMAND_XYZ\n");
    assert(cmd.type == CMD_UNKNOWN);

    printf("  -> Bluetooth parser passed all assertions.\n");
    g_tests_passed++;
}

void test_soil_moisture_calibration() {
    printf("[TEST] Running Soil Moisture Calibration Tests...\n");

    uint16_t air_dry = 800;
    uint16_t water_wet = 300;

    /* Completely dry */
    assert(calculate_soil_moisture_pct(800, air_dry, water_wet) == 0);
    assert(calculate_soil_moisture_pct(900, air_dry, water_wet) == 0);

    /* Fully saturated */
    assert(calculate_soil_moisture_pct(300, air_dry, water_wet) == 100);
    assert(calculate_soil_moisture_pct(200, air_dry, water_wet) == 100);

    /* Mid-point (550 ADC) */
    uint8_t mid = calculate_soil_moisture_pct(550, air_dry, water_wet);
    assert(mid == 50);

    printf("  -> Soil moisture calibration passed all assertions.\n");
    g_tests_passed++;
}

void test_irrigation_hysteresis_and_watchdog() {
    printf("[TEST] Running Irrigation Hysteresis & Safety Watchdog Tests...\n");

    irrigation_controller_t irr;
    irrigation_init(&irr, 35, 65, 15000, 5000);

    uint32_t t = 1000;

    /* Soil is 50% (Normal) -> Pump should be OFF */
    assert(irrigation_update(&irr, 50, t) == false);

    /* Soil drops to 30% (< 35%) -> Pump should turn ON */
    t += 1000;
    assert(irrigation_update(&irr, 30, t) == true);
    assert(irr.state == PUMP_STATE_WATERING);

    /* Soil rises to 45% (Between 35% and 65%) -> Pump should REMAIN ON (Hysteresis) */
    t += 1000;
    assert(irrigation_update(&irr, 45, t) == true);

    /* Soil rises to 68% (>= 65%) -> Pump should turn OFF */
    t += 1000;
    assert(irrigation_update(&irr, 68, t) == false);
    assert(irr.state == PUMP_STATE_COOLDOWN);

    /* SAFETY WATCHDOG TEST: Simulate sensor detachment where pump runs past 15,000ms */
    t += 6000; /* Cooldown finished */
    irrigation_update(&irr, 20, t); /* Re-triggers watering */
    assert(irr.pump_active == true);

    /* Advance time by 16,000ms */
    t += 16000;
    bool active = irrigation_update(&irr, 20, t);
    assert(active == false);
    assert(irr.emergency_tripped == true);
    assert(irr.state == PUMP_STATE_SAFETY_TRIP);

    printf("  -> Irrigation hysteresis & safety watchdog passed all assertions.\n");
    g_tests_passed++;
}

int main(void) {
    printf("====================================================\n");
    printf(" SMART HOME AUTOMATION UNIT TEST SUITE (NATIVE C++)\n");
    printf("====================================================\n");

    test_bluetooth_command_parser();
    test_soil_moisture_calibration();
    test_irrigation_hysteresis_and_watchdog();

    printf("====================================================\n");
    printf(" ALL %d TEST SUITES PASSED CLEANLY (100%% SUCCESS)\n", g_tests_passed);
    printf("====================================================\n");
    return 0;
}
