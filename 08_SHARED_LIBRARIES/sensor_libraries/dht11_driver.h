#ifndef DHT11_DRIVER_H
#define DHT11_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DHT_OK = 0,
    DHT_ERR_TIMEOUT = -1,
    DHT_ERR_CHECKSUM = -2,
    DHT_ERR_PARAM = -3
} dht_status_t;

typedef struct {
    float temperature_c;
    float humidity_pct;
    bool is_valid;
} dht11_data_t;

/**
 * @brief Decode raw 40-bit DHT11 payload into temperature and humidity.
 *
 * Byte 0: Integral Relative Humidity
 * Byte 1: Decimal Relative Humidity
 * Byte 2: Integral Temperature
 * Byte 3: Decimal Temperature
 * Byte 4: Checksum (Sum of Bytes 0-3)
 */
dht_status_t dht11_decode_bytes(const uint8_t raw[5], dht11_data_t *out_data);

#ifdef __cplusplus
}
#endif

#endif /* DHT11_DRIVER_H */
