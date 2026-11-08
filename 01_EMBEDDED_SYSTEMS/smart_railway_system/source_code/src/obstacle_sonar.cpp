/**
 * @file obstacle_sonar.cpp
 * @brief Track obstacle ultrasonic sonar sensor implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "obstacle_sonar.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
static void pinMode(uint8_t, uint8_t) {}
static void digitalWrite(uint8_t, uint8_t) {}
static void delayMicroseconds(unsigned int) {}
static unsigned long pulseIn(uint8_t, uint8_t, unsigned long) { return 5820; /* ~100cm clear */ }
#define OUTPUT 1
#define INPUT  0
#define HIGH   1
#define LOW    0
#endif

void obstacle_sonar_init(ObstacleSonar_t *sonar, uint8_t trig, uint8_t echo) {
    if (!sonar) return;

    sonar->trig_pin = trig;
    sonar->echo_pin = echo;
    sonar->last_distance_cm = TRACK_CLEAR_MIN_CM;

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trig, LOW);
}

uint16_t obstacle_sonar_ping(ObstacleSonar_t *sonar) {
    if (!sonar) return 0;

    digitalWrite(sonar->trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(sonar->trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sonar->trig_pin, LOW);

    unsigned long duration_us = pulseIn(sonar->echo_pin, HIGH, 30000UL);

    if (duration_us == 0) {
        sonar->last_distance_cm = 400; // No obstacle within 4 meters
    } else {
        sonar->last_distance_cm = (uint16_t)(duration_us / 58UL);
    }

    return sonar->last_distance_cm;
}
