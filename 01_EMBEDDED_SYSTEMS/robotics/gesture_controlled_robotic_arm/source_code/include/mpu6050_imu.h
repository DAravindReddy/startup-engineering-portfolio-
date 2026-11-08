/**
 * @file mpu6050_imu.h
 * @brief MPU6050 6-Axis Inertial Measurement Unit driver with Complementary Filter.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef MPU6050_IMU_H
#define MPU6050_IMU_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int16_t accel_x_raw;
    int16_t accel_y_raw;
    int16_t accel_z_raw;
    int16_t gyro_x_raw;
    int16_t gyro_y_raw;
    int16_t gyro_z_raw;
    float roll_deg;    // Angle around X-axis (-90 to +90)
    float pitch_deg;   // Angle around Y-axis (-90 to +90)
    float yaw_rate;    // Angular velocity around Z-axis (deg/s)
    uint32_t last_sample_ms;
} Mpu6050_t;

bool mpu6050_init(Mpu6050_t *imu, uint8_t i2c_addr);

bool mpu6050_read_raw(Mpu6050_t *imu);

void mpu6050_update_filter(Mpu6050_t *imu, uint32_t current_time_ms);

#ifdef __cplusplus
}
#endif

#endif // MPU6050_IMU_H
