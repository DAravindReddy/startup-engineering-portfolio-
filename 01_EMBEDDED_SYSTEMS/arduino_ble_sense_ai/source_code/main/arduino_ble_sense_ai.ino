/**
 * @file arduino_ble_sense_ai.ino
 * @brief Arduino Nano 33 BLE Sense TinyML Predictive Maintenance Main Sketch.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "config.h"
#include "sensor_manager.h"
#include "feature_extractor.h"
#include "tinyml_anomaly_detector.h"
#include "ble_telemetry_service.h"

// System state instances
static SensorData_t g_sensor_data;
static FeatureExtractor_t g_feature_extractor;
static MachineFeatures_t g_features;
static AnomalyInference_t g_inference;
static BleTelemetryService_t g_ble;

// Non-blocking loop timing
static uint32_t g_last_imu_sample = 0;
static uint32_t g_last_env_sample = 0;
static uint32_t g_last_inference = 0;
static uint32_t g_last_telemetry = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[INIT] Arduino Nano 33 BLE Sense TinyML Health Monitor initializing..."));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    sensor_manager_init();
    feature_extractor_init(&g_feature_extractor);
    tinyml_detector_init();
    ble_telemetry_init(&g_ble);

    Serial.println(F("[INIT] TinyML engine initialized. BLE advertising active."));
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    uint32_t now = millis();

    // 1. Maintain BLE stack servicing
    ble_telemetry_poll(&g_ble);

    // 2. High-speed 100 Hz IMU vibration sampling (Every 10 ms)
    if (now - g_last_imu_sample >= (1000 / IMU_SAMPLE_RATE_HZ)) {
        g_last_imu_sample = now;
        if (sensor_manager_read_imu(&g_sensor_data)) {
            feature_extractor_push_sample(&g_feature_extractor, g_sensor_data.accel_mag);
        }
    }

    // 3. 1 Hz Environmental sensor polling (Every 1000 ms)
    if (now - g_last_env_sample >= ENV_SAMPLE_INTERVAL_MS) {
        g_last_env_sample = now;
        sensor_manager_read_env(&g_sensor_data);
    }

    // 4. 2 Hz TinyML Anomaly Inference & BLE Notification (Every 500 ms)
    if (now - g_last_inference >= INFERENCE_INTERVAL_MS) {
        g_last_inference = now;

        if (feature_extractor_compute(&g_feature_extractor, g_sensor_data.temperature_c, now, &g_features)) {
            tinyml_detector_predict(&g_features, &g_inference);

            // Broadcast to connected BLE clients (Central)
            ble_telemetry_broadcast(&g_ble, g_inference.anomaly_score_pct,
                                    g_features.vibration_rms,
                                    g_features.mean_temperature_c,
                                    g_inference.state);
        }
    }

    // 5. 1 Hz Debug Telemetry Serial Print
    if (now - g_last_telemetry >= 1000) {
        g_last_telemetry = now;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        Serial.print(F("[TELEMETRY] Score: "));
        Serial.print(g_inference.anomaly_score_pct, 1);
        Serial.print(F("% | State: "));
        Serial.print(g_inference.state == MACHINE_STATE_NORMAL ? F("NORMAL") :
                    (g_inference.state == MACHINE_STATE_WARNING ? F("WARNING") : F("FAULT")));
        Serial.print(F(" | RMS: "));
        Serial.print(g_features.vibration_rms, 2);
        Serial.print(F(" m/s² | Temp: "));
        Serial.print(g_features.mean_temperature_c, 1);
        Serial.print(F("°C | BLE: "));
        Serial.println(g_ble.client_connected ? F("CONNECTED") : F("ADVERTISING"));
    }
}
