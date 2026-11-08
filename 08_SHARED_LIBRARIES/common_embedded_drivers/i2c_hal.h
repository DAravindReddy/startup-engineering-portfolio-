#ifndef I2C_HAL_H
#define I2C_HAL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    I2C_OK = 0,
    I2C_ERR_NACK = -1,
    I2C_ERR_TIMEOUT = -2,
    I2C_ERR_PARAM = -3,
    I2C_ERR_BUS_BUSY = -4
} i2c_status_t;

typedef struct {
    uint32_t clock_speed_hz;
    uint32_t timeout_ms;
    uint8_t retry_count;
} i2c_config_t;

/**
 * @brief Initialize the hardware I2C peripheral.
 */
i2c_status_t i2c_init(const i2c_config_t *config);

/**
 * @brief Write bytes to a target 7-bit slave address.
 */
i2c_status_t i2c_write(uint8_t slave_addr, const uint8_t *data, uint16_t len);

/**
 * @brief Read bytes from a target 7-bit slave address.
 */
i2c_status_t i2c_read(uint8_t slave_addr, uint8_t *buffer, uint16_t len);

/**
 * @brief Write to a register address and read response in repeated start condition.
 */
i2c_status_t i2c_write_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* I2C_HAL_H */
