/**
 * @file mpu6050_imu.cpp
 * @brief MPU6050 6-Axis Inertial Measurement Unit implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "mpu6050_imu.h"
#include <math.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#endif

#define MPU_REG_PWR_MGMT_1   0x6B
#define MPU_REG_ACCEL_XOUT_H 0x3B
#define RAD_TO_DEG           57.2957795f

bool mpu6050_init(Mpu6050_t *imu, uint8_t i2c_addr) {
    if (!imu) return false;

    imu->accel_x_raw = 0;
    imu->accel_y_raw = 0;
    imu->accel_z_raw = 16384; // 1G on Z-axis for 2G scale
    imu->gyro_x_raw = 0;
    imu->gyro_y_raw = 0;
    imu->gyro_z_raw = 0;
    imu->roll_deg = 0.0f;
    imu->pitch_deg = 0.0f;
    imu->yaw_rate = 0.0f;
    imu->last_sample_ms = 0;

#ifdef ARDUINO
    Wire.begin();
    Wire.beginTransmission(i2c_addr);
    Wire.write(MPU_REG_PWR_MGMT_1);
    Wire.write(0x00); // Wake up MPU6050 from sleep
    uint8_t err = Wire.endTransmission();
    return (err == 0);
#else
    (void)i2c_addr;
    return true;
#endif
}

bool mpu6050_read_raw(Mpu6050_t *imu) {
    if (!imu) return false;

#ifdef ARDUINO
    Wire.beginTransmission(MPU6050_I2C_ADDR);
    Wire.write(MPU_REG_ACCEL_XOUT_H);
    if (Wire.endTransmission(false) != 0) return false;

    // Request 14 bytes: 6 accel, 2 temp, 6 gyro
    Wire.requestFrom((uint8_t)MPU6050_I2C_ADDR, (uint8_t)14);
    if (Wire.available() < 14) return false;

    imu->accel_x_raw = (int16_t)(Wire.read() << 8 | Wire.read());
    imu->accel_y_raw = (int16_t)(Wire.read() << 8 | Wire.read());
    imu->accel_z_raw = (int16_t)(Wire.read() << 8 | Wire.read());
    (void)(Wire.read() << 8 | Wire.read()); // Discard temperature
    imu->gyro_x_raw  = (int16_t)(Wire.read() << 8 | Wire.read());
    imu->gyro_y_raw  = (int16_t)(Wire.read() << 8 | Wire.read());
    imu->gyro_z_raw  = (int16_t)(Wire.read() << 8 | Wire.read());
    return true;
#else
    return true;
#endif
}

void mpu6050_update_filter(Mpu6050_t *imu, uint32_t current_time_ms) {
    if (!imu) return;

    float dt = (current_time_ms - imu->last_sample_ms) / 1000.0f;
    if (dt <= 0.0f || dt > 0.5f) dt = 0.02f; // Clamp to 20ms fallback
    imu->last_sample_ms = current_time_ms;

    // Convert raw accelerometer to roll and pitch angles in degrees
    float ax = (float)imu->accel_x_raw;
    float ay = (float)imu->accel_y_raw;
    float az = (float)imu->accel_z_raw;

    float accel_roll = atan2f(ay, sqrtf(ax * ax + az * az)) * RAD_TO_DEG;
    float accel_pitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD_TO_DEG;

    // Convert raw gyro to deg/sec (FS_SEL = 0 -> 131.0 LSB / (deg/s))
    float gyro_roll_rate  = (float)imu->gyro_x_raw / 131.0f;
    float gyro_pitch_rate = (float)imu->gyro_y_raw / 131.0f;
    imu->yaw_rate         = (float)imu->gyro_z_raw / 131.0f;

    // Complementary Filter: High-pass gyro + Low-pass accelerometer
    imu->roll_deg  = IMU_COMPLEMENTARY_ALPHA * (imu->roll_deg + gyro_roll_rate * dt) +
                     (1.0f - IMU_COMPLEMENTARY_ALPHA) * accel_roll;

    imu->pitch_deg = IMU_COMPLEMENTARY_ALPHA * (imu->pitch_deg + gyro_pitch_rate * dt) +
                     (1.0f - IMU_COMPLEMENTARY_ALPHA) * accel_pitch;
}
