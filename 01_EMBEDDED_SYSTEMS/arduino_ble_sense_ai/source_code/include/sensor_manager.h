/**
 * @file sensor_manager.h
 * @brief Onboard IMU accelerometer and environmental sensor manager.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float ax;
    float ay;
    float az;
    float accel_mag; // Vector magnitude sqrt(ax^2 + ay^2 + az^2)
    float temperature_c;
    float humidity_pct;
    bool imu_healthy;
    bool env_healthy;
} SensorData_t;

void sensor_manager_init(void);

bool sensor_manager_read_imu(SensorData_t *data);

bool sensor_manager_read_env(SensorData_t *data);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_MANAGER_H
