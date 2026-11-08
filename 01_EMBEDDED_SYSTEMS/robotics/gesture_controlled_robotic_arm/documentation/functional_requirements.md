# Gesture Controlled Robotic Arm - Functional Requirements & Software Design

## 1. Functional Requirements

- **FR-01 (Continuous IMU Sampling)**: The system shall sample the MPU6050 6-axis accelerometer and gyroscope over I2C at a rate of 50 Hz (every 20 ms).
- **FR-02 (Sensor Fusion Orientation)**: The software shall implement a complementary orientation filter combining rate-gyro integration and gravitational accelerometer vector projection with $\alpha = 0.96$.
- **FR-03 (Roll-to-Base Joint Translation)**: Operator hand roll within $[-50^\circ, +50^\circ]$ shall map linearly to Base Pan joint rotation between $[10^\circ, 170^\circ]$.
- **FR-04 (Pitch-to-Shoulder Joint Translation)**: Operator hand pitch within $[-45^\circ, +45^\circ]$ shall map linearly to Shoulder joint elevation between $[20^\circ, 160^\circ]$.
- **FR-05 (Horizon Articulation Compensation)**: The Elbow joint shall automatically compensate for shoulder elevation to maintain horizontal end-effector attitude during reaching maneuvers.
- **FR-06 (End-Effector Gripper Clamping)**: Pressing the glove grip trigger button shall actuate the claw clamp between $40^\circ$ (Open) and $115^\circ$ (Clamped).
- **FR-07 (Velocity Slew-Rate Limiting)**: Joint angular velocity across all 4 servos shall be strictly capped at $\le 90^\circ/\text{sec}$ to eliminate mechanical shock.

---

## 2. Software Design & Implementation Architecture

```text
source_code/
├── include/
│   ├── config.h               # Joint limits, slew rate limits, I2C addresses
│   ├── mpu6050_imu.h          # 6-axis IMU driver and complementary filter
│   ├── servo_controller.h     # 4-DOF servo management with slew-rate profiler
│   └── kinematics_mapper.h    # Gesture-to-joint coordinate mapping
└── src/
    ├── mpu6050_imu.cpp
    ├── servo_controller.cpp
    └── kinematics_mapper.cpp
```
