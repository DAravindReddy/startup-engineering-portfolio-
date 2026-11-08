/**
 * @file sensor_manager.cpp
 * @brief Onboard IMU accelerometer and environmental sensor implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "sensor_manager.h"
#include <math.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>
#endif

void sensor_manager_init(void) {
#ifdef ARDUINO
    IMU.begin();
    HTS.begin();
#endif
}

bool sensor_manager_read_imu(SensorData_t *data) {
    if (!data) return false;

#ifdef ARDUINO
    float x = 0, y = 0, z = 0;
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        // Convert from G to m/s² (1 G ≈ 9.80665 m/s²)
        data->ax = x * 9.80665f;
        data->ay = y * 9.80665f;
        data->az = z * 9.80665f;
        data->accel_mag = sqrtf(data->ax * data->ax + data->ay * data->ay + data->az * data->az);
        data->imu_healthy = true;
        return true;
    }
    return false;
#else
    // Desktop simulation fallback
    data->ax = 0.2f;
    data->ay = 0.1f;
    data->az = 9.81f;
    data->accel_mag = 9.81f;
    data->imu_healthy = true;
    return true;
#endif
}

bool sensor_manager_read_env(SensorData_t *data) {
    if (!data) return false;

#ifdef ARDUINO
    data->temperature_c = HTS.readTemperature();
    data->humidity_pct = HTS.readHumidity();
    data->env_healthy = true;
    return true;
#else
    data->temperature_c = 42.5f;
    data->humidity_pct = 48.0f;
    data->env_healthy = true;
    return true;
#endif
}
