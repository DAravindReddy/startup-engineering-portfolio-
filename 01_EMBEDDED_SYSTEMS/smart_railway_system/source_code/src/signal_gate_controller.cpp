/**
 * @file signal_gate_controller.cpp
 * @brief Railway traffic signals, level-crossing barrier gate, and alert buzzer implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "signal_gate_controller.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <Servo.h>
static Servo g_gate_servo;
#else
static void pinMode(uint8_t, uint8_t) {}
static void digitalWrite(uint8_t, uint8_t) {}
#define OUTPUT 1
#define HIGH   1
#define LOW    0
#endif

void signal_gate_init(SignalGateController_t *ctrl, uint8_t pin_g, uint8_t pin_y,
                      uint8_t pin_r, uint8_t pin_servo, uint8_t pin_buzz) {
    if (!ctrl) return;

    ctrl->pin_green = pin_g;
    ctrl->pin_yellow = pin_y;
    ctrl->pin_red = pin_r;
    ctrl->pin_gate_servo = pin_servo;
    ctrl->pin_buzzer = pin_buzz;
    ctrl->current_aspect = SIGNAL_ASPECT_GREEN;
    ctrl->current_gate_angle = GATE_ANGLE_OPEN;
    ctrl->buzzer_active = false;

    pinMode(pin_g, OUTPUT);
    pinMode(pin_y, OUTPUT);
    pinMode(pin_r, OUTPUT);
    pinMode(pin_buzz, OUTPUT);

#ifdef ARDUINO
    g_gate_servo.attach(pin_servo);
    g_gate_servo.write(GATE_ANGLE_OPEN);
#endif

    signal_gate_set_aspect(ctrl, SIGNAL_ASPECT_GREEN);
}

void signal_gate_set_aspect(SignalGateController_t *ctrl, SignalAspect_t aspect) {
    if (!ctrl) return;

    ctrl->current_aspect = aspect;

    switch (aspect) {
        case SIGNAL_ASPECT_GREEN:
            digitalWrite(ctrl->pin_green, HIGH);
            digitalWrite(ctrl->pin_yellow, LOW);
            digitalWrite(ctrl->pin_red, LOW);
            digitalWrite(ctrl->pin_buzzer, LOW);
            ctrl->buzzer_active = false;
            signal_gate_set_barrier(ctrl, GATE_ANGLE_OPEN);
            break;

        case SIGNAL_ASPECT_YELLOW:
            digitalWrite(ctrl->pin_green, LOW);
            digitalWrite(ctrl->pin_yellow, HIGH);
            digitalWrite(ctrl->pin_red, LOW);
            digitalWrite(ctrl->pin_buzzer, LOW);
            ctrl->buzzer_active = false;
            signal_gate_set_barrier(ctrl, GATE_ANGLE_OPEN);
            break;

        case SIGNAL_ASPECT_RED:
        default:
            digitalWrite(ctrl->pin_green, LOW);
            digitalWrite(ctrl->pin_yellow, LOW);
            digitalWrite(ctrl->pin_red, HIGH);
            digitalWrite(ctrl->pin_buzzer, HIGH);
            ctrl->buzzer_active = true;
            signal_gate_set_barrier(ctrl, GATE_ANGLE_CLOSED);
            break;
    }
}

void signal_gate_set_barrier(SignalGateController_t *ctrl, uint8_t angle_deg) {
    if (!ctrl) return;

    if (angle_deg > 90) angle_deg = 90;
    ctrl->current_gate_angle = angle_deg;

#ifdef ARDUINO
    g_gate_servo.write(angle_deg);
#endif
}
