/**
 * @file ultrasonic_sensor.cpp
 * @brief HC-SR04 Sonar distance measurement driver implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "ultrasonic_sensor.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
static void pinMode(uint8_t, uint8_t) {}
static void digitalWrite(uint8_t, uint8_t) {}
static void delayMicroseconds(unsigned int) {}
static unsigned long pulseIn(uint8_t, uint8_t, unsigned long) { return 1740; /* ~30cm */ }
#define OUTPUT 1
#define INPUT  0
#define HIGH   1
#define LOW    0
#endif

void ultrasonic_init(UltrasonicSensor_t *sensor, uint8_t trig, uint8_t echo) {
    if (!sensor) return;

    sensor->trig_pin = trig;
    sensor->echo_pin = echo;
    sensor->last_distance_cm = DISTANCE_CLEAR_CM;
    sensor->last_trigger_ms = 0;

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trig, LOW);
}

uint16_t ultrasonic_ping_blocking(UltrasonicSensor_t *sensor) {
    if (!sensor) return 0;

    // Send 10 microsecond trigger pulse
    digitalWrite(sensor->trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(sensor->trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor->trig_pin, LOW);

    // Measure echo pulse width (timeout ~25ms -> ~430cm)
    unsigned long duration_us = pulseIn(sensor->echo_pin, HIGH, 25000UL);

    if (duration_us == 0) {
        // Echo timeout: no obstruction detected within range
        return DISTANCE_MAX_VALID_CM;
    }

    // Speed of sound at sea level ~343 m/s = 0.0343 cm/us
    // Distance = (duration / 2) * 0.0343 = duration / 58.2
    uint16_t distance_cm = (uint16_t)(duration_us / 58UL);

    if (distance_cm > DISTANCE_MAX_VALID_CM) {
        distance_cm = DISTANCE_MAX_VALID_CM;
    }

    return ultrasonic_filter_sample(sensor, distance_cm);
}

uint16_t ultrasonic_filter_sample(UltrasonicSensor_t *sensor, uint16_t raw_cm) {
    if (!sensor) return raw_cm;

    // First sample initialization
    if (sensor->last_distance_cm == 0) {
        sensor->last_distance_cm = raw_cm;
        return raw_cm;
    }

    // Exponential moving average filter: 70% current, 30% previous
    uint16_t filtered = (uint16_t)((raw_cm * 7 + sensor->last_distance_cm * 3) / 10);
    sensor->last_distance_cm = filtered;
    return filtered;
}
