#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool led1_state;
    bool led2_state;
    bool motor_enabled;
    uint8_t motor_speed;
    uint8_t servo_angle;
    bool pump_relay_state;
} actuator_states_t;

#ifdef __cplusplus
extern "C" {
#endif

void actuator_controller_init(actuator_states_t *actuators);
void set_led1(actuator_states_t *actuators, bool on);
void set_led2(actuator_states_t *actuators, bool on);
void set_motor_speed(actuator_states_t *actuators, uint8_t speed);
void set_servo_angle(actuator_states_t *actuators, uint8_t angle);
void set_pump_relay(actuator_states_t *actuators, bool on);

#ifdef __cplusplus
}
#endif

#endif /* ACTUATOR_CONTROLLER_H */
