/**
 * @file servo_scanner.h
 * @brief SG90 Servo pan mechanism for directional sonar obstacle scanning.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef SERVO_SCANNER_H
#define SERVO_SCANNER_H

#include <stdint.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t pin;
    uint8_t current_angle;
    uint32_t last_move_ms;
} ServoScanner_t;

void servo_scanner_init(ServoScanner_t *scanner, uint8_t pin);

void servo_scanner_set_angle(ServoScanner_t *scanner, uint8_t angle);

uint8_t servo_scanner_get_angle(const ServoScanner_t *scanner);

#ifdef __cplusplus
}
#endif

#endif // SERVO_SCANNER_H
