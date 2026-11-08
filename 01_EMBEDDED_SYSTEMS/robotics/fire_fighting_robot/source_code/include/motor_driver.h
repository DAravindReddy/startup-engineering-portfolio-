/**
 * @file motor_driver.h
 * @brief Differential drive H-bridge motor driver for Fire Fighting Robot.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DRIVE_STOP = 0,
    DRIVE_FORWARD,
    DRIVE_REVERSE,
    DRIVE_PIVOT_LEFT,
    DRIVE_PIVOT_RIGHT,
    DRIVE_BRAKE
} DriveDirection_t;

typedef struct {
    uint8_t pin_ena;
    uint8_t pin_in1;
    uint8_t pin_in2;
    uint8_t pin_in3;
    uint8_t pin_in4;
    uint8_t pin_enb;
    DriveDirection_t current_direction;
    uint8_t current_speed;
} MotorDriver_t;

void motor_driver_init(MotorDriver_t *driver, uint8_t ena, uint8_t in1, uint8_t in2,
                       uint8_t in3, uint8_t in4, uint8_t enb);

void motor_driver_set_motion(MotorDriver_t *driver, DriveDirection_t dir, uint8_t speed);

void motor_driver_stop(MotorDriver_t *driver);

#ifdef __cplusplus
}
#endif

#endif // MOTOR_DRIVER_H
