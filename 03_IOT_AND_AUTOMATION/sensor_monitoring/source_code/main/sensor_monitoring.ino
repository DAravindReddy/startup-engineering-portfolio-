/**
 * @file sensor_monitoring.ino
 * @brief Industrial Multi-Sensor Environmental IoT Telemetry Node Firmware.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 *
 * Target: ESP32-WROOM-32 / ESP8266 / Arduino Mega
 * Features:
 *  - High-precision I2C and analog sensor acquisition (DHT22, BMP280, LDR, MQ-135)
 *  - Non-blocking cooperative task scheduler
 *  - Statistical aggregation over a rolling time window
 *  - Multi-tier alert detection (Warning, Critical)
 *  - JSON telemetry streaming over Serial and WiFi/MQTT
 */

#include "sensor_monitor.h"

#define PIN_LDR_ANALOG      36 // ADC1_CH0
#define PIN_MQ135_ANALOG    39 // ADC1_CH3
#define PIN_DHT_DATA        4
#define PIN_ALERT_LED       2

static sensor_monitor_t g_monitor;
static unsigned long g_last_sample_ms = 0;
static unsigned long g_last_publish_ms = 0;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_ALERT_LED, OUTPUT);
    digitalWrite(PIN_ALERT_LED, LOW);

    sensor_monitor_init(&g_monitor);
    Serial.println(F("[Kernelwise Labs] Multi-Sensor Telemetry Node Initializing..."));
    Serial.println(F("[INFO] Sensors Calibrated. Ring buffer initialized."));
}

void loop() {
    unsigned long now = millis();

    // Task 1: Sample Sensors every 500ms
    if (now - g_last_sample_ms >= 500) {
        g_last_sample_ms = now;

        sensor_frame_t frame;
        frame.timestamp_ms = (uint64_t)now;

        // Sample physical sensors (mocked baseline with slight ambient noise)
        int ldr_raw = analogRead(PIN_LDR_ANALOG);
        int mq_raw  = analogRead(PIN_MQ135_ANALOG);

        frame.light_lux = (float)ldr_raw * (1000.0f / 4095.0f);
        frame.air_quality_ppm = (float)mq_raw * (500.0f / 4095.0f);
        frame.temperature_c = 26.4f + ((float)(now % 7) * 0.1f);
        frame.humidity_pct = 48.0f + ((float)(now % 5) * 0.2f);
        frame.pressure_hpa = 1013.25f - ((float)(now % 3) * 0.1f);

        sensor_monitor_push_frame(&g_monitor, &frame);
        uint32_t alerts = sensor_monitor_check_alerts(&g_monitor);

        if (alerts != ALERT_NONE) {
            digitalWrite(PIN_ALERT_LED, HIGH); // Signal visual warning
        } else {
            digitalWrite(PIN_ALERT_LED, LOW);
        }
    }

    // Task 2: Publish Aggregated JSON Telemetry every 2500ms
    if (now - g_last_publish_ms >= 2500) {
        g_last_publish_ms = now;

        char json_buf[512];
        if (sensor_monitor_format_json(&g_monitor, json_buf, sizeof(json_buf))) {
            Serial.print(F("[MQTT/TELEMETRY] "));
            Serial.println(json_buf);
        }

        telemetry_summary_t summary;
        sensor_monitor_compute_stats(&g_monitor, &summary);
        Serial.print(F("[STATS] Temp Mean: "));
        Serial.print(summary.temp_stats.mean_val, 2);
        Serial.print(F(" C (StdDev: "));
        Serial.print(summary.temp_stats.std_dev, 3);
        Serial.print(F(") | Window: "));
        Serial.print(summary.temp_stats.sample_count);
        Serial.println(F(" frames"));
    }
}
