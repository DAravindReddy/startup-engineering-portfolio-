/**
 * @file ble_telemetry_service.h
 * @brief Bluetooth Low Energy (BLE) GATT Machine Health Service interface.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef BLE_TELEMETRY_SERVICE_H
#define BLE_TELEMETRY_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "tinyml_anomaly_detector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool ble_initialized;
    bool client_connected;
    uint32_t last_notify_ms;
} BleTelemetryService_t;

void ble_telemetry_init(BleTelemetryService_t *ble);

void ble_telemetry_poll(BleTelemetryService_t *ble);

void ble_telemetry_broadcast(BleTelemetryService_t *ble, float health_score,
                             float vibration_rms, float temperature, MachineState_t state);

#ifdef __cplusplus
}
#endif

#endif // BLE_TELEMETRY_SERVICE_H
