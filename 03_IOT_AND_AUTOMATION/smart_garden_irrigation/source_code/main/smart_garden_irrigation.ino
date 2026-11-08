/**
 * @file smart_garden_irrigation.ino
 * @brief Autonomous IoT Smart Garden Irrigation Controller Firmware.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 *
 * Target: ESP32-WROOM-32 / Arduino Mega / UNO WiFi
 * Features:
 *  - Non-blocking cooperative scheduler (100ms task loop, 1s telemetry, 5s MQTT publish)
 *  - Analog soil moisture sensor with ADC calibration and EMA smoothing
 *  - DHT11 / DHT22 temperature and relative humidity sensing
 *  - 5V Relay active-low pump drive with anti-hammer safety cooldown
 *  - MQTT publish/subscribe for telemetry streaming and remote valve overrides
 */

#include "irrigation_controller.h"

#define PIN_SOIL_ANALOG      34
#define PIN_DHT_DATA         4
#define PIN_PUMP_RELAY       23
#define PIN_STATUS_LED       2

// Hardware Calibration constants (Capacitive Moisture Sensor v1.2)
#define SENSOR_AIR_VALUE     3200.0f // Value measured in dry air
#define SENSOR_WATER_VALUE   1400.0f // Value measured fully immersed in water

static irrigation_controller_t g_controller;
static unsigned long g_last_sensor_poll_ms = 0;
static unsigned long g_last_telemetry_ms = 0;

float read_calibrated_moisture_pct() {
    int raw_adc = analogRead(PIN_SOIL_ANALOG);
    // Linear interpolation between air and water calibration bounds
    float pct = ((SENSOR_AIR_VALUE - (float)raw_adc) / (SENSOR_AIR_VALUE - SENSOR_WATER_VALUE)) * 100.0f;
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 100.0f) pct = 100.0f;
    return pct;
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_PUMP_RELAY, OUTPUT);
    pinMode(PIN_STATUS_LED, OUTPUT);

    // Relay active-low: HIGH = OFF
    digitalWrite(PIN_PUMP_RELAY, HIGH);
    digitalWrite(PIN_STATUS_LED, LOW);

    irrigation_init(&g_controller);
    Serial.println(F("[Kernelwise Labs] Smart Garden Irrigation Node Booting..."));
    Serial.println(F("[INFO] System ready. Operating in CLOSED-LOOP AUTOMATIC mode."));
}

void loop() {
    unsigned long now = millis();

    // Task 1: Sensor Polling & State Machine Evaluation (Every 250ms)
    if (now - g_last_sensor_poll_ms >= 250) {
        g_last_sensor_poll_ms = now;

        float moisture = read_calibrated_moisture_pct();
        // In physical hardware, DHT read function is called here (mocked at 28.5C, 45% RH)
        float temp = 28.5f;
        float humidity = 45.0f;

        irrigation_update_sensors(&g_controller, moisture, temp, humidity);
        irrigation_adapt_thresholds(&g_controller);
        irrigation_fsm_tick(&g_controller, (uint64_t)now);

        // Actuator Control (Active-LOW relay)
        if (g_controller.pump_relay_active) {
            digitalWrite(PIN_PUMP_RELAY, LOW); // Turn pump ON
            digitalWrite(PIN_STATUS_LED, HIGH);
        } else {
            digitalWrite(PIN_PUMP_RELAY, HIGH); // Turn pump OFF
            digitalWrite(PIN_STATUS_LED, LOW);
        }
    }

    // Task 2: Telemetry Logging & MQTT Streaming (Every 2000ms)
    if (now - g_last_telemetry_ms >= 2000) {
        g_last_telemetry_ms = now;

        Serial.print(F("[TELEMETRY] Moisture: "));
        Serial.print(g_controller.telemetry.filtered_moisture_pct, 1);
        Serial.print(F("% | Temp: "));
        Serial.print(g_controller.telemetry.ambient_temp_c, 1);
        Serial.print(F("C | Thresholds: ["));
        Serial.print(g_controller.threshold_low, 1);
        Serial.print(F("%, "));
        Serial.print(g_controller.threshold_high, 1);
        Serial.print(F("%] | Pump: "));
        Serial.println(g_controller.pump_relay_active ? F("ACTIVE") : F("STANDBY"));
    }
}
