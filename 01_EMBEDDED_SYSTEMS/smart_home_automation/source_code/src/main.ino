/**
 * @file main.ino
 * @brief Smart Home Automation with Bluetooth & Closed-Loop Irrigation
 * @author Aravind Reddy (DAravindReddy)
 *
 * Architecture: Non-blocking cooperative multitasking loop with millis()
 * Silicon: Arduino UNO (ATmega328P)
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include "../include/config.h"
#include "../include/bluetooth_handler.h"
#include "../include/sensor_manager.h"
#include "../include/actuator_controller.h"
#include "../include/irrigation_system.h"

/* Peripheral Objects */
SoftwareSerial btSerial(PIN_BT_RX, PIN_BT_TX); /* RX, TX */
Servo windowServo;

/* System State Models */
sensor_readings_t sensors;
actuator_states_t actuators;
irrigation_controller_t irrigation;

/* Command Ingestion Buffer */
char rx_buffer[BT_CMD_BUFFER_MAX];
uint8_t rx_idx = 0;

/* Telemetry & Timing Trackers */
uint32_t last_telemetry_broadcast = 0;
uint32_t last_heartbeat_toggle = 0;
bool heartbeat_state = false;

/* Simple non-blocking DHT11 Bit-Bang Read Mock / Real Driver Hook */
void sample_dht11_sensor() {
    /* Simple analog/digital timing sample */
    /* On physical hardware, uses DHT library or shared dht11_decode_bytes() */
    sensors.temperature_c = 26.5f;
    sensors.humidity_pct = 54.0f;
    sensors.dht_valid = true;
    sensors.last_dht_read_ms = millis();
}

void sample_soil_sensor() {
    uint16_t raw_adc = analogRead(PIN_SOIL_MOISTURE_ANALOG);
    sensors.soil_moisture_raw_adc = raw_adc;
    sensors.soil_moisture_pct = calculate_soil_moisture_pct(raw_adc, SOIL_RAW_AIR_DRY, SOIL_RAW_WATER_WET);
    sensors.last_soil_read_ms = millis();
}

void apply_hardware_actuators() {
    /* LEDs */
    digitalWrite(PIN_LED_APPLIANCE_1, actuators.led1_state ? HIGH : LOW);
    digitalWrite(PIN_LED_APPLIANCE_2, actuators.led2_state ? HIGH : LOW);

    /* DC Motor via L298N */
    if (actuators.motor_enabled && actuators.motor_speed > 0) {
        digitalWrite(PIN_MOTOR_IN1, HIGH);
        digitalWrite(PIN_MOTOR_IN2, LOW);
        analogWrite(PIN_MOTOR_PWM_ENA, actuators.motor_speed);
    } else {
        digitalWrite(PIN_MOTOR_IN1, LOW);
        digitalWrite(PIN_MOTOR_IN2, LOW);
        analogWrite(PIN_MOTOR_PWM_ENA, 0);
    }

    /* Servo Damper / Window Position */
    windowServo.write(actuators.servo_angle);

    /* Water Pump Relay (Active LOW relay module standard) */
    /* LOW = Energized (Pump ON), HIGH = De-energized (Pump OFF) */
    digitalWrite(PIN_WATER_PUMP_RELAY, actuators.pump_relay_state ? LOW : HIGH);
}

void send_telemetry_status(Stream &out) {
    out.print(F("{\"temp\":"));
    out.print(sensors.temperature_c, 1);
    out.print(F(",\"hum\":"));
    out.print(sensors.humidity_pct, 1);
    out.print(F(",\"soil_pct\":"));
    out.print(sensors.soil_moisture_pct);
    out.print(F(",\"pump\":"));
    out.print(actuators.pump_relay_state ? 1 : 0);
    out.print(F(",\"mode\":\""));
    out.print(irrigation.mode == IRRIGATION_MODE_AUTO ? F("AUTO") : F("MANUAL"));
    out.print(F("\",\"led1\":"));
    out.print(actuators.led1_state ? 1 : 0);
    out.print(F(",\"led2\":"));
    out.print(actuators.led2_state ? 1 : 0);
    out.print(F(",\"motor_spd\":"));
    out.print(actuators.motor_speed);
    out.print(F(",\"servo\":"));
    out.print(actuators.servo_angle);
    out.println(F("}"));
}

void execute_command(bt_parsed_command_t cmd) {
    switch (cmd.type) {
        case CMD_LED1_ON:
            set_led1(&actuators, true);
            btSerial.println(F("[ACK] LED1 ON"));
            break;
        case CMD_LED1_OFF:
            set_led1(&actuators, false);
            btSerial.println(F("[ACK] LED1 OFF"));
            break;
        case CMD_LED2_ON:
            set_led2(&actuators, true);
            btSerial.println(F("[ACK] LED2 ON"));
            break;
        case CMD_LED2_OFF:
            set_led2(&actuators, false);
            btSerial.println(F("[ACK] LED2 OFF"));
            break;
        case CMD_MOTOR_ON:
            set_motor_speed(&actuators, 200);
            btSerial.println(F("[ACK] MOTOR ON (SPEED 200)"));
            break;
        case CMD_MOTOR_OFF:
            set_motor_speed(&actuators, 0);
            btSerial.println(F("[ACK] MOTOR OFF"));
            break;
        case CMD_MOTOR_SPEED:
            set_motor_speed(&actuators, (uint8_t)cmd.param_val);
            btSerial.print(F("[ACK] MOTOR SPEED: "));
            btSerial.println(cmd.param_val);
            break;
        case CMD_SERVO_ANGLE:
            set_servo_angle(&actuators, (uint8_t)cmd.param_val);
            btSerial.print(F("[ACK] SERVO ANGLE: "));
            btSerial.println(cmd.param_val);
            break;
        case CMD_PUMP_ON:
            irrigation_manual_start(&irrigation, millis());
            btSerial.println(F("[ACK] MANUAL PUMP ON"));
            break;
        case CMD_PUMP_OFF:
            irrigation_manual_stop(&irrigation, millis());
            btSerial.println(F("[ACK] MANUAL PUMP OFF"));
            break;
        case CMD_MODE_AUTO:
            irrigation_set_mode(&irrigation, IRRIGATION_MODE_AUTO);
            btSerial.println(F("[ACK] IRRIGATION MODE: AUTO"));
            break;
        case CMD_MODE_MANUAL:
            irrigation_set_mode(&irrigation, IRRIGATION_MODE_MANUAL);
            btSerial.println(F("[ACK] IRRIGATION MODE: MANUAL"));
            break;
        case CMD_SET_THRESHOLD:
            irrigation_set_thresholds(&irrigation, (uint8_t)cmd.param_val, (uint8_t)(cmd.param_val + 20));
            btSerial.print(F("[ACK] SOIL DRY THRESHOLD SET TO: "));
            btSerial.println(cmd.param_val);
            break;
        case CMD_GET_STATUS:
            send_telemetry_status(btSerial);
            send_telemetry_status(Serial);
            break;
        case CMD_UNKNOWN:
            btSerial.println(F("[ERR] UNKNOWN COMMAND. Type STATUS? for telemetry"));
            break;
        case CMD_NONE:
        default:
            break;
    }
}

void process_serial_input(Stream &stream) {
    while (stream.available()) {
        char c = (char)stream.read();
        if (c == '\r' || c == '\n') {
            if (rx_idx > 0) {
                rx_buffer[rx_idx] = '\0';
                bt_parsed_command_t cmd = parse_bluetooth_command(rx_buffer);
                execute_command(cmd);
                rx_idx = 0;
            }
        } else if (rx_idx < (BT_CMD_BUFFER_MAX - 1)) {
            rx_buffer[rx_idx++] = c;
        }
    }
}

void setup() {
    /* Hardware GPIO Setup */
    pinMode(PIN_LED_APPLIANCE_1, OUTPUT);
    pinMode(PIN_LED_APPLIANCE_2, OUTPUT);
    pinMode(PIN_STATUS_LED, OUTPUT);
    pinMode(PIN_MOTOR_PWM_ENA, OUTPUT);
    pinMode(PIN_MOTOR_IN1, OUTPUT);
    pinMode(PIN_MOTOR_IN2, OUTPUT);
    pinMode(PIN_WATER_PUMP_RELAY, OUTPUT);
    pinMode(PIN_SOIL_MOISTURE_ANALOG, INPUT);

    /* Safe Initial Output States */
    digitalWrite(PIN_WATER_PUMP_RELAY, HIGH); /* Relay De-energized */
    digitalWrite(PIN_LED_APPLIANCE_1, LOW);
    digitalWrite(PIN_LED_APPLIANCE_2, LOW);
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    analogWrite(PIN_MOTOR_PWM_ENA, 0);

    /* Attach Servo */
    windowServo.attach(PIN_SERVO_PWM);
    windowServo.write(90);

    /* Subsystem Model Initialization */
    sensor_manager_init(&sensors);
    actuator_controller_init(&actuators);
    irrigation_init(&irrigation, DEFAULT_SOIL_DRY_THRESHOLD_PCT, DEFAULT_SOIL_WET_THRESHOLD_PCT,
                    PUMP_SAFETY_MAX_RUN_TIME_MS, PUMP_MIN_REST_TIME_MS);

    /* Communication Buses */
    Serial.begin(SERIAL_BAUD_RATE);
    btSerial.begin(BLUETOOTH_BAUD_RATE);

    Serial.println(F("=================================================="));
    Serial.println(F("Smart Home Automation & Irrigation Subsystem"));
    Serial.println(F("System Online. Ready for Bluetooth commands."));
    Serial.println(F("=================================================="));
}

void loop() {
    uint32_t now = millis();

    /* 1. Ingest commands from Bluetooth and USB Serial */
    process_serial_input(btSerial);
    process_serial_input(Serial);

    /* 2. Sensor sampling schedules */
    if (now - sensors.last_soil_read_ms >= INTERVAL_SOIL_READ_MS) {
        sample_soil_sensor();
    }
    if (now - sensors.last_dht_read_ms >= INTERVAL_DHT_READ_MS) {
        sample_dht11_sensor();
    }

    /* 3. Closed-loop irrigation logic */
    bool pump_demand = irrigation_update(&irrigation, sensors.soil_moisture_pct, now);
    set_pump_relay(&actuators, pump_demand);

    /* 4. Drive physical hardware pins */
    apply_hardware_actuators();

    /* 5. Heartbeat Blink (1 Hz) */
    if (now - last_heartbeat_toggle >= 500) {
        last_heartbeat_toggle = now;
        heartbeat_state = !heartbeat_state;
        digitalWrite(PIN_STATUS_LED, heartbeat_state ? HIGH : LOW);
    }

    /* 6. Periodic Telemetry Broadcast */
    if (now - last_telemetry_broadcast >= INTERVAL_TELEMETRY_MS) {
        last_telemetry_broadcast = now;
        send_telemetry_status(btSerial);
    }
}
