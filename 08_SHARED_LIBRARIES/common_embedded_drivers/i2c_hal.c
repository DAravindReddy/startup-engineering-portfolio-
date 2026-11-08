/**
 * @file i2c_hal.c
 * @brief Portable Hardware Abstraction Layer implementation for I2C master operations.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "i2c_hal.h"
#include <string.h>

static i2c_config_t g_i2c_cfg = {
    .clock_speed_hz = 100000,
    .timeout_ms = 100,
    .retry_count = 3
};
static bool g_i2c_initialized = false;

i2c_status_t i2c_init(const i2c_config_t *config) {
    if (!config) return I2C_ERR_PARAM;
    if (config->clock_speed_hz == 0) return I2C_ERR_PARAM;

    g_i2c_cfg = *config;
    g_i2c_initialized = true;
    return I2C_OK;
}

i2c_status_t i2c_write(uint8_t slave_addr, const uint8_t *data, uint16_t len) {
    if (!g_i2c_initialized) return I2C_ERR_BUS_BUSY;
    if (!data || len == 0) return I2C_ERR_PARAM;
    if (slave_addr > 0x7F) return I2C_ERR_PARAM;

    // Platform-independent HAL stub: Hardware register writes map here
    return I2C_OK;
}

i2c_status_t i2c_read(uint8_t slave_addr, uint8_t *buffer, uint16_t len) {
    if (!g_i2c_initialized) return I2C_ERR_BUS_BUSY;
    if (!buffer || len == 0) return I2C_ERR_PARAM;
    if (slave_addr > 0x7F) return I2C_ERR_PARAM;

    // Zero-fill buffer for deterministic read behavior
    memset(buffer, 0, len);
    return I2C_OK;
}

i2c_status_t i2c_write_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *buffer, uint16_t len) {
    i2c_status_t status = i2c_write(slave_addr, &reg_addr, 1);
    if (status != I2C_OK) return status;
    return i2c_read(slave_addr, buffer, len);
}
