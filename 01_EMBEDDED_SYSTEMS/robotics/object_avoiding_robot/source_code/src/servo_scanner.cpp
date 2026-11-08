/**
 * @file servo_scanner.cpp
 * @brief SG90 Servo pan mechanism implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "servo_scanner.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <Servo.h>
static Servo g_servo;
#endif

void servo_scanner_init(ServoScanner_t *scanner, uint8_t pin) {
    if (!scanner) return;

    scanner->pin = pin;
    scanner->current_angle = SERVO_ANGLE_CENTER;
    scanner->last_move_ms = 0;

#ifdef ARDUINO
    g_servo.attach(pin);
    g_servo.write(SERVO_ANGLE_CENTER);
#endif
}

void servo_scanner_set_angle(ServoScanner_t *scanner, uint8_t angle) {
    if (!scanner) return;

    if (angle > 180) angle = 180;
    scanner->current_angle = angle;

#ifdef ARDUINO
    g_servo.write(angle);
#endif
}

uint8_t servo_scanner_get_angle(const ServoScanner_t *scanner) {
    return scanner ? scanner->current_angle : SERVO_ANGLE_CENTER;
}
