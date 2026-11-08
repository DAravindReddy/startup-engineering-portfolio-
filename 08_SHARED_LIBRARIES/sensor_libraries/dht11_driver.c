#include "dht11_driver.h"

dht_status_t dht11_decode_bytes(const uint8_t raw[5], dht11_data_t *out_data) {
    if (!raw || !out_data) {
        return DHT_ERR_PARAM;
    }

    uint8_t calculated_checksum = (uint8_t)(raw[0] + raw[1] + raw[2] + raw[3]);
    if (calculated_checksum != raw[4]) {
        out_data->is_valid = false;
        return DHT_ERR_CHECKSUM;
    }

    out_data->humidity_pct = (float)raw[0] + ((float)raw[1] * 0.1f);
    out_data->temperature_c = (float)raw[2] + ((float)raw[3] * 0.1f);
    out_data->is_valid = true;

    return DHT_OK;
}
