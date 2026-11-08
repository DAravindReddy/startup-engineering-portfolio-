/**
 * @file config.h
 * @brief Hardware Pinout and System Configuration for Smart Home Automation
 * @author Aravind Reddy (DAravindReddy)
 *
 * NOTE FOR PHYSICAL HARDWARE VERIFICATION:
 * Pin assignments below reflect standard Arduino UNO layout.
 * Verify these connections with your specific shield or wiring before powering on.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* ========================================================================== */
/* PIN DEFINITIONS                                                            */
/* ========================================================================== */

/* Bluetooth Communication (SoftwareSerial / AltSoftSerial) */
/* Hardware Note: HC-05 TX connects to Arduino RX, HC-05 RX connects to Arduino TX via 1k/2k voltage divider */
#define PIN_BT_RX               2   /* Arduino RX (from HC-05 TXD) */
#define PIN_BT_TX               3   /* Arduino TX (to HC-05 RXD via level divider) */

/* Environmental & Soil Sensors */
#define PIN_DHT11_DATA          4   /* DHT11 1-Wire Data pin (requires 4.7k-10k pull-up) */
#define PIN_SOIL_MOISTURE_ANALOG A0  /* Analog Soil Moisture Sensor output */
#define PIN_SOIL_MOISTURE_DIGITAL 5 /* Optional Digital comparator threshold */

/* Actuators - Motors and Servos */
#define PIN_SERVO_PWM           9   /* SG90 Servo PWM Signal pin (Timer1 PWM) */
#define PIN_MOTOR_PWM_ENA       6   /* L298N Enable A (PWM Speed Control) */
#define PIN_MOTOR_IN1           7   /* L298N Direction Input 1 */
#define PIN_MOTOR_IN2           8   /* L298N Direction Input 2 */

/* Actuators - Relays and Irrigation Pump */
#define PIN_WATER_PUMP_RELAY    10  /* Active-LOW or Active-HIGH Relay signal for DC Water Pump */
#define PIN_LED_APPLIANCE_1     11  /* Appliance 1 / Room LED */
#define PIN_LED_APPLIANCE_2     12  /* Appliance 2 / Yard LED */
#define PIN_STATUS_LED          13  /* Onboard Status / Heartbeat LED */

/* ========================================================================== */
/* OPERATIONAL CONSTANTS & TIMING (NON-BLOCKING)                              */
/* ========================================================================== */

#define SERIAL_BAUD_RATE        9600
#define BLUETOOTH_BAUD_RATE     9600

/* Sensor Polling Intervals */
#define INTERVAL_DHT_READ_MS    2000  /* DHT11 minimum refresh cycle (2 seconds) */
#define INTERVAL_SOIL_READ_MS   1000  /* Soil moisture polling interval */
#define INTERVAL_TELEMETRY_MS   3000  /* Periodic Bluetooth status broadcast */

/* Irrigation Configuration */
#define DEFAULT_SOIL_DRY_THRESHOLD_PCT  35  /* Turn ON pump when moisture < 35% */
#define DEFAULT_SOIL_WET_THRESHOLD_PCT  65  /* Turn OFF pump when moisture > 65% (Hysteresis) */
#define PUMP_SAFETY_MAX_RUN_TIME_MS     15000 /* Auto-shutoff after 15s to prevent overflow */
#define PUMP_MIN_REST_TIME_MS           5000  /* Prevent rapid cycling */

/* Soil Sensor Calibration Values (Raw 10-bit ADC 0 - 1023) */
#define SOIL_RAW_AIR_DRY        820   /* ADC reading in dry air (0% moisture) */
#define SOIL_RAW_WATER_WET      310   /* ADC reading immersed in water (100% moisture) */

#endif /* CONFIG_H */
