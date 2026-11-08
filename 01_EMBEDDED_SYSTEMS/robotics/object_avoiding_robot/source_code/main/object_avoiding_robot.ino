/**
 * @file object_avoiding_robot.ino
 * @brief Autonomous Object Avoiding Robot Main Arduino Sketch.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "config.h"
#include "motor_driver.h"
#include "ultrasonic_sensor.h"
#include "servo_scanner.h"
#include "navigation_fsm.h"

// Hardware driver instances
static MotorDriver_t g_motors;
static UltrasonicSensor_t g_sonar;
static ServoScanner_t g_scanner;
static NavigationFSM_t g_fsm;

// Non-blocking loop scheduler
static uint32_t g_last_fsm_tick = 0;
static uint32_t g_last_heartbeat = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[INIT] Autonomous Object Avoiding Robot initializing..."));

    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, HIGH);

    motor_driver_init(&g_motors, PIN_MOTOR_ENA, PIN_MOTOR_IN1, PIN_MOTOR_IN2,
                      PIN_MOTOR_IN3, PIN_MOTOR_IN4, PIN_MOTOR_ENB);

    ultrasonic_init(&g_sonar, PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO);
    servo_scanner_init(&g_scanner, PIN_SERVO_PWM);

    nav_fsm_init(&g_fsm, &g_motors, &g_sonar, &g_scanner);

    Serial.println(F("[INIT] Setup complete. Entering autonomous cruise loop."));
    digitalWrite(PIN_STATUS_LED, LOW);
}

void loop() {
    uint32_t now = millis();

    // 50 Hz Navigation FSM update rate (every 20 ms)
    if (now - g_last_fsm_tick >= 20) {
        g_last_fsm_tick = now;
        nav_fsm_update(&g_fsm, now);
    }

    // 1 Hz Status LED heartbeat & telemetry output
    if (now - g_last_heartbeat >= 1000) {
        g_last_heartbeat = now;
        digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));

        Serial.print(F("[TELEMETRY] State: "));
        Serial.print((int)nav_fsm_get_state(&g_fsm));
        Serial.print(F(" | Fwd: "));
        Serial.print(g_fsm.dist_forward);
        Serial.print(F(" cm | Left: "));
        Serial.print(g_fsm.dist_left);
        Serial.print(F(" cm | Right: "));
        Serial.print(g_fsm.dist_right);
        Serial.println(F(" cm"));
    }
}
