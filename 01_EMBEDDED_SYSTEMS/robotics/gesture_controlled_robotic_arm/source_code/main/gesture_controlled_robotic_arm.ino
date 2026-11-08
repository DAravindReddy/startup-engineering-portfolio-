/**
 * @file gesture_controlled_robotic_arm.ino
 * @brief Gesture Controlled 4-DOF Robotic Arm Main Arduino Sketch.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <Wire.h>
#include "config.h"
#include "mpu6050_imu.h"
#include "servo_controller.h"
#include "kinematics_mapper.h"

// Hardware and logic instances
static Mpu6050_t g_imu;
static ServoController_t g_servos;
static KinematicsMapper_t g_mapper;

// Non-blocking timer loops
static uint32_t g_last_control_tick = 0;
static uint32_t g_last_telemetry = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[INIT] Gesture Controlled 4-DOF Robotic Arm initializing..."));

    pinMode(PIN_GRIPPER_BUTTON, INPUT_PULLUP);
    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, HIGH);

    // Initialize 4 joint servos
    servo_controller_init(&g_servos, PIN_SERVO_BASE, PIN_SERVO_SHOULDER,
                          PIN_SERVO_ELBOW, PIN_SERVO_GRIPPER);

    // Initialize Kinematics Mapper
    kinematics_init(&g_mapper);

    // Initialize MPU6050 6-Axis IMU
    Wire.begin();
    if (!mpu6050_init(&g_imu, MPU6050_I2C_ADDR)) {
        Serial.println(F("[ERROR] MPU6050 IMU connection failed! Check I2C wiring."));
    } else {
        Serial.println(F("[INIT] MPU6050 IMU initialized successfully."));
    }

    Serial.println(F("[INIT] System ready. Commencing gesture tracking loop."));
    digitalWrite(PIN_STATUS_LED, LOW);
}

void loop() {
    uint32_t now = millis();

    // 50 Hz Control & Motion Loop (Every 20 ms)
    if (now - g_last_control_tick >= SAMPLING_PERIOD_MS) {
        g_last_control_tick = now;

        // 1. Read IMU raw acceleration and angular rates
        if (mpu6050_read_raw(&g_imu)) {
            mpu6050_update_filter(&g_imu, now);
        }

        // 2. Read clamp trigger button (Active-LOW with internal pullup)
        bool grip_pressed = (digitalRead(PIN_GRIPPER_BUTTON) == LOW);

        // 3. Compute joint angles via Kinematic Mapper
        kinematics_map_gesture(&g_mapper, g_imu.roll_deg, g_imu.pitch_deg, grip_pressed);

        // 4. Update servo targets and execute smooth slew-rate motion step
        servo_controller_set_targets(&g_servos, g_mapper.joint_base_deg,
                                     g_mapper.joint_shoulder_deg,
                                     g_mapper.joint_elbow_deg,
                                     g_mapper.joint_gripper_deg);

        servo_controller_update(&g_servos, now);
    }

    // 1 Hz Heartbeat & Telemetry Serial Stream
    if (now - g_last_telemetry >= 1000) {
        g_last_telemetry = now;
        digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));

        Serial.print(F("[TELEMETRY] Roll: "));
        Serial.print(g_imu.roll_deg, 1);
        Serial.print(F("° | Pitch: "));
        Serial.print(g_imu.pitch_deg, 1);
        Serial.print(F("° || Base: "));
        Serial.print((int)g_servos.current_base);
        Serial.print(F("° | Shldr: "));
        Serial.print((int)g_servos.current_shoulder);
        Serial.print(F("° | Elbow: "));
        Serial.print((int)g_servos.current_elbow);
        Serial.print(F("° | Grip: "));
        Serial.print((int)g_servos.current_gripper);
        Serial.println(F("°"));
    }
}
