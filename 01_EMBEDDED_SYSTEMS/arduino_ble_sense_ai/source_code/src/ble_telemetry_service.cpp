/**
 * @file ble_telemetry_service.cpp
 * @brief Bluetooth Low Energy (BLE) GATT Machine Health Service implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "ble_telemetry_service.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <ArduinoBLE.h>

static BLEService s_health_service(BLE_UUID_HEALTH_SERVICE);
static BLEFloatCharacteristic s_char_score(BLE_UUID_CHAR_HEALTH_SCORE, BLERead | BLENotify);
static BLEFloatCharacteristic s_char_vibration(BLE_UUID_CHAR_VIBRATION_RMS, BLERead | BLENotify);
static BLEFloatCharacteristic s_char_temperature(BLE_UUID_CHAR_TEMPERATURE, BLERead | BLENotify);
static BLEByteCharacteristic s_char_state(BLE_UUID_CHAR_MACHINE_STATE, BLERead | BLENotify);
#endif

void ble_telemetry_init(BleTelemetryService_t *ble) {
    if (!ble) return;

    ble->ble_initialized = false;
    ble->client_connected = false;
    ble->last_notify_ms = 0;

#ifdef ARDUINO
    if (!BLE.begin()) {
        return;
    }

    BLE.setLocalName(BLE_DEVICE_NAME);
    BLE.setAdvertisedService(s_health_service);

    s_health_service.addCharacteristic(s_char_score);
    s_health_service.addCharacteristic(s_char_vibration);
    s_health_service.addCharacteristic(s_char_temperature);
    s_health_service.addCharacteristic(s_char_state);

    BLE.addService(s_health_service);

    s_char_score.writeValue(0.0f);
    s_char_vibration.writeValue(0.0f);
    s_char_temperature.writeValue(25.0f);
    s_char_state.writeValue((uint8_t)0);

    BLE.advertise();
    ble->ble_initialized = true;
#endif
}

void ble_telemetry_poll(BleTelemetryService_t *ble) {
    if (!ble) return;

#ifdef ARDUINO
    BLE.poll();
    BLEDevice central = BLE.central();
    ble->client_connected = central ? central.connected() : false;
#endif
}

void ble_telemetry_broadcast(BleTelemetryService_t *ble, float health_score,
                             float vibration_rms, float temperature, MachineState_t state) {
    if (!ble) return;

#ifdef ARDUINO
    if (ble->ble_initialized) {
        s_char_score.writeValue(health_score);
        s_char_vibration.writeValue(vibration_rms);
        s_char_temperature.writeValue(temperature);
        s_char_state.writeValue((uint8_t)state);
    }
#else
    (void)health_score;
    (void)vibration_rms;
    (void)temperature;
    (void)state;
#endif
}
