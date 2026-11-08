/**
 * @file ultrasonic_sensor.h
 * @brief HC-SR04 Sonar distance measurement driver with filtering.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t trig_pin;
    uint8_t echo_pin;
    uint16_t last_distance_cm;
    uint32_t last_trigger_ms;
} UltrasonicSensor_t;

void ultrasonic_init(UltrasonicSensor_t *sensor, uint8_t trig, uint8_t echo);

uint16_t ultrasonic_ping_blocking(UltrasonicSensor_t *sensor);

uint16_t ultrasonic_filter_sample(UltrasonicSensor_t *sensor, uint16_t raw_cm);

#ifdef __cplusplus
}
#endif

#endif // ULTRASONIC_SENSOR_H
