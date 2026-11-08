/**
 * @file motor_driver.cpp
 * @brief Differential drive H-bridge motor controller implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "motor_driver.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
// Mock definitions for desktop unit testing
static void pinMode(uint8_t, uint8_t) {}
static void digitalWrite(uint8_t, uint8_t) {}
static void analogWrite(uint8_t, uint8_t) {}
#define OUTPUT 1
#define HIGH   1
#define LOW    0
#endif

void motor_driver_init(MotorDriver_t *driver, uint8_t ena, uint8_t in1, uint8_t in2,
                       uint8_t in3, uint8_t in4, uint8_t enb) {
    if (!driver) return;

    driver->pin_ena = ena;
    driver->pin_in1 = in1;
    driver->pin_in2 = in2;
    driver->pin_in3 = in3;
    driver->pin_in4 = in4;
    driver->pin_enb = enb;
    driver->current_direction = DRIVE_STOP;
    driver->current_speed = 0;

    pinMode(ena, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enb, OUTPUT);

    motor_driver_stop(driver);
}

void motor_driver_set_motion(MotorDriver_t *driver, DriveDirection_t dir, uint8_t speed) {
    if (!driver) return;

    driver->current_direction = dir;
    driver->current_speed = speed;

    switch (dir) {
        case DRIVE_FORWARD:
            // Left motor forward, Right motor forward
            digitalWrite(driver->pin_in1, HIGH);
            digitalWrite(driver->pin_in2, LOW);
            digitalWrite(driver->pin_in3, HIGH);
            digitalWrite(driver->pin_in4, LOW);
            analogWrite(driver->pin_ena, speed);
            analogWrite(driver->pin_enb, speed);
            break;

        case DRIVE_REVERSE:
            // Left motor backward, Right motor backward
            digitalWrite(driver->pin_in1, LOW);
            digitalWrite(driver->pin_in2, HIGH);
            digitalWrite(driver->pin_in3, LOW);
            digitalWrite(driver->pin_in4, HIGH);
            analogWrite(driver->pin_ena, speed);
            analogWrite(driver->pin_enb, speed);
            break;

        case DRIVE_PIVOT_LEFT:
            // Left motor backward, Right motor forward
            digitalWrite(driver->pin_in1, LOW);
            digitalWrite(driver->pin_in2, HIGH);
            digitalWrite(driver->pin_in3, HIGH);
            digitalWrite(driver->pin_in4, LOW);
            analogWrite(driver->pin_ena, speed);
            analogWrite(driver->pin_enb, speed);
            break;

        case DRIVE_PIVOT_RIGHT:
            // Left motor forward, Right motor backward
            digitalWrite(driver->pin_in1, HIGH);
            digitalWrite(driver->pin_in2, LOW);
            digitalWrite(driver->pin_in3, LOW);
            digitalWrite(driver->pin_in4, HIGH);
            analogWrite(driver->pin_ena, speed);
            analogWrite(driver->pin_enb, speed);
            break;

        case DRIVE_BRAKE:
        case DRIVE_STOP:
        default:
            digitalWrite(driver->pin_in1, LOW);
            digitalWrite(driver->pin_in2, LOW);
            digitalWrite(driver->pin_in3, LOW);
            digitalWrite(driver->pin_in4, LOW);
            analogWrite(driver->pin_ena, 0);
            analogWrite(driver->pin_enb, 0);
            driver->current_speed = 0;
            break;
    }
}

void motor_driver_stop(MotorDriver_t *driver) {
    motor_driver_set_motion(driver, DRIVE_STOP, 0);
}
