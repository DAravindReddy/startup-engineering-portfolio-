/**
 * @file obstacle_sonar.h
 * @brief Track obstacle ultrasonic sonar sensor interface.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef OBSTACLE_SONAR_H
#define OBSTACLE_SONAR_H

#include <stdint.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t trig_pin;
    uint8_t echo_pin;
    uint16_t last_distance_cm;
} ObstacleSonar_t;

void obstacle_sonar_init(ObstacleSonar_t *sonar, uint8_t trig, uint8_t echo);

uint16_t obstacle_sonar_ping(ObstacleSonar_t *sonar);

#ifdef __cplusplus
}
#endif

#endif // OBSTACLE_SONAR_H
