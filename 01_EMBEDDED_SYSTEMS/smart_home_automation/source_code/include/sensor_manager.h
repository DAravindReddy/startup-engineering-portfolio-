#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float temperature_c;
    float humidity_pct;
    uint16_t soil_moisture_raw_adc;
    uint8_t soil_moisture_pct;
    bool dht_valid;
    uint32_t last_dht_read_ms;
    uint32_t last_soil_read_ms;
} sensor_readings_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize sensor data structure with default calibration.
 */
void sensor_manager_init(sensor_readings_t *sensors);

/**
 * @brief Converts raw ADC reading into calibrated soil moisture percentage.
 */
uint8_t calculate_soil_moisture_pct(uint16_t raw_adc, uint16_t air_dry_adc, uint16_t water_wet_adc);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MANAGER_H */
