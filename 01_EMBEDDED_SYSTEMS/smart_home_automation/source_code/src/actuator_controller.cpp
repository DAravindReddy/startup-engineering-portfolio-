#include "actuator_controller.h"

void actuator_controller_init(actuator_states_t *actuators) {
    if (!actuators) return;
    actuators->led1_state = false;
    actuators->led2_state = false;
    actuators->motor_enabled = false;
    actuators->motor_speed = 0;
    actuators->servo_angle = 90;
    actuators->pump_relay_state = false;
}

void set_led1(actuator_states_t *actuators, bool on) {
    if (!actuators) return;
    actuators->led1_state = on;
}

void set_led2(actuator_states_t *actuators, bool on) {
    if (!actuators) return;
    actuators->led2_state = on;
}

void set_motor_speed(actuator_states_t *actuators, uint8_t speed) {
    if (!actuators) return;
    actuators->motor_speed = speed;
    actuators->motor_enabled = (speed > 0);
}

void set_servo_angle(actuator_states_t *actuators, uint8_t angle) {
    if (!actuators) return;
    if (angle > 180) angle = 180;
    actuators->servo_angle = angle;
}

void set_pump_relay(actuator_states_t *actuators, bool on) {
    if (!actuators) return;
    actuators->pump_relay_state = on;
}
