/**
 * @file fire_fighting_robot.ino
 * @brief Autonomous Fire Fighting Robot Main Arduino Sketch.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "config.h"
#include "motor_driver.h"
#include "flame_sensor_array.h"
#include "pump_controller.h"
#include "fire_fighting_fsm.h"

// Hardware instances
static MotorDriver_t g_motors;
static FlameSensorArray_t g_flame_sensors;
static PumpController_t g_pump;
static FireFightingFSM_t g_fsm;

// Non-blocking timer loops
static uint32_t g_last_fsm_tick = 0;
static uint32_t g_last_telemetry = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[INIT] Autonomous Fire Fighting Robot initializing..."));

    pinMode(PIN_FIRE_ALARM_BUZZER, OUTPUT);
    digitalWrite(PIN_FIRE_ALARM_BUZZER, LOW);

    motor_driver_init(&g_motors, PIN_MOTOR_ENA, PIN_MOTOR_IN1, PIN_MOTOR_IN2,
                      PIN_MOTOR_IN3, PIN_MOTOR_IN4, PIN_MOTOR_ENB);

    flame_sensors_init(&g_flame_sensors, PIN_FLAME_LEFT, PIN_FLAME_CENTER, PIN_FLAME_RIGHT);
    pump_controller_init(&g_pump, PIN_PUMP_RELAY);

    ff_fsm_init(&g_fsm, &g_motors, &g_flame_sensors, &g_pump);

    Serial.println(F("[INIT] System armed. Entering autonomous fire patrol."));
}

void loop() {
    uint32_t now = millis();

    // 50 Hz FSM execution loop (20 ms interval)
    if (now - g_last_fsm_tick >= 20) {
        g_last_fsm_tick = now;
        ff_fsm_update(&g_fsm, now);
    }

    // 1 Hz Status Telemetry & Strobe
    if (now - g_last_telemetry >= 1000) {
        g_last_telemetry = now;

        FireFightingState_t st = ff_fsm_get_state(&g_fsm);
        bool is_fire = (st == FF_STATE_ALIGN_LEFT || st == FF_STATE_ALIGN_RIGHT ||
                        st == FF_STATE_APPROACH_FIRE || st == FF_STATE_EXTINGUISH_SPRAY);

        digitalWrite(PIN_FIRE_ALARM_BUZZER, is_fire ? HIGH : LOW);

        Serial.print(F("[TELEMETRY] State: "));
        Serial.print((int)st);
        Serial.print(F(" | FlamePos: "));
        Serial.print((int)g_fsm.detected_flame_pos);
        Serial.print(F(" | PumpActive: "));
        Serial.print(pump_controller_is_running(&g_pump) ? 1 : 0);
        Serial.print(F(" | Sprays: "));
        Serial.println(g_pump.consecutive_spray_count);
    }
}
