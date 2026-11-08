#include "sensor_manager.h"
#include <string.h>

void sensor_manager_init(sensor_readings_t *sensors) {
    if (!sensors) return;
    sensors->temperature_c = 25.0f;
    sensors->humidity_pct = 50.0f;
    sensors->soil_moisture_raw_adc = 600;
    sensors->soil_moisture_pct = 50;
    sensors->dht_valid = false;
    sensors->last_dht_read_ms = 0;
    sensors->last_soil_read_ms = 0;
}

uint8_t calculate_soil_moisture_pct(uint16_t raw_adc, uint16_t air_dry_adc, uint16_t water_wet_adc) {
    if (air_dry_adc <= water_wet_adc) {
        return 0; /* Invalid calibration bounds */
    }

    if (raw_adc >= air_dry_adc) {
        return 0; /* Completely dry */
    }
    if (raw_adc <= water_wet_adc) {
        return 100; /* Fully saturated */
    }

    /* Inverse linear mapping: Higher ADC = Drier soil */
    uint32_t span = air_dry_adc - water_wet_adc;
    uint32_t val = air_dry_adc - raw_adc;
    uint8_t pct = (uint8_t)((val * 100UL) / span);
    return pct;
}
