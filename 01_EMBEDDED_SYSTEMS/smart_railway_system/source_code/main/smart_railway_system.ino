/**
 * @file smart_railway_system.ino
 * @brief Smart Railway Track Crack and Obstacle Detection System Main Arduino Sketch.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "config.h"
#include "crack_detector.h"
#include "obstacle_sonar.h"
#include "signal_gate_controller.h"
#include "railway_fsm.h"

// Hardware driver instances
static CrackDetector_t g_crack_detector;
static ObstacleSonar_t g_sonar;
static SignalGateController_t g_signals;
static RailwayFSM_t g_fsm;

// Non-blocking timer schedulers
static uint32_t g_last_fsm_tick = 0;
static uint32_t g_last_telemetry = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[INIT] Smart Railway Track Safety & Inspection System initializing..."));

    // Optional inspection trolley motor pins
    pinMode(PIN_TROLLEY_MOTOR_ENA, OUTPUT);
    pinMode(PIN_TROLLEY_MOTOR_IN1, OUTPUT);
    pinMode(PIN_TROLLEY_MOTOR_IN2, OUTPUT);

    // Drive trolley forward in inspection mode
    digitalWrite(PIN_TROLLEY_MOTOR_IN1, HIGH);
    digitalWrite(PIN_TROLLEY_MOTOR_IN2, LOW);
    analogWrite(PIN_TROLLEY_MOTOR_ENA, SPEED_INSPECTION_CRUISE);

    // Initialize subsystems
    crack_detector_init(&g_crack_detector, PIN_CRACK_LEFT_RAIL, PIN_CRACK_RIGHT_RAIL);
    obstacle_sonar_init(&g_sonar, PIN_SONAR_TRIG, PIN_SONAR_ECHO);
    signal_gate_init(&g_signals, PIN_SIGNAL_GREEN, PIN_SIGNAL_YELLOW,
                     PIN_SIGNAL_RED, PIN_GATE_SERVO, PIN_ALARM_BUZZER);

    railway_fsm_init(&g_fsm, &g_crack_detector, &g_sonar, &g_signals);

    Serial.println(F("[INIT] Track safety system armed. Commencing monitoring loop."));
}

void loop() {
    uint32_t now = millis();

    // 20 Hz FSM Update Loop (every 50 ms)
    if (now - g_last_fsm_tick >= SAMPLING_PERIOD_MS) {
        g_last_fsm_tick = now;
        railway_fsm_update(&g_fsm, now);

        // Safety interlock: Stop trolley on any Red Danger condition
        RailwayFSMState_t state = railway_fsm_get_state(&g_fsm);
        if (state == RAIL_FSM_STATE_CRACK_DANGER || state == RAIL_FSM_STATE_OBSTACLE_DANGER) {
            analogWrite(PIN_TROLLEY_MOTOR_ENA, SPEED_EMERGENCY_HALT);
        } else if (state == RAIL_FSM_STATE_CAUTION) {
            analogWrite(PIN_TROLLEY_MOTOR_ENA, (SPEED_INSPECTION_CRUISE / 2));
        } else {
            analogWrite(PIN_TROLLEY_MOTOR_ENA, SPEED_INSPECTION_CRUISE);
        }
    }

    // 1 Hz Status Telemetry Serial Dispatch
    if (now - g_last_telemetry >= 1000) {
        g_last_telemetry = now;

        RailwayFSMState_t state = railway_fsm_get_state(&g_fsm);

        Serial.print(F("[TELEMETRY] State: "));
        Serial.print((int)state);
        Serial.print(F(" | Aspect: "));
        Serial.print(g_signals.current_aspect == SIGNAL_ASPECT_GREEN ? F("GREEN") :
                    (g_signals.current_aspect == SIGNAL_ASPECT_YELLOW ? F("YELLOW") : F("RED")));
        Serial.print(F(" | Gate: "));
        Serial.print(g_signals.current_gate_angle);
        Serial.print(F("° | ObstacleDist: "));
        Serial.print(g_fsm.last_obstacle_dist_cm);
        Serial.print(F(" cm | CrackStatus: "));
        Serial.println((int)g_fsm.last_crack_status);
    }
}
