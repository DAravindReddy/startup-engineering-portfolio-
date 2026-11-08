# Gesture Controlled Robotic Arm - Pin Configuration & Signal Mapping

## 1. Arduino UNO Microcontroller Pin Assignments

| Arduino Pin | Signal Function | Target Peripheral | Subsystem | Pin Mode | Logic Level |
|:---:|:---|:---|:---|:---:|:---:|
| **D2** | Grip Toggle Input | Pushbutton / Flex Sensor | Operator Input | `INPUT_PULLUP` | 5V TTL (Active-LOW) |
| **D3** | Base Servo PWM (Axis 1) | MG996R Base Pan Servo | Motion Output | `OUTPUT` (PWM) | 5V TTL |
| **D5** | Shoulder Servo PWM (Axis 2)| MG996R Shoulder Lift Servo | Motion Output | `OUTPUT` (PWM) | 5V TTL |
| **D6** | Elbow Servo PWM (Axis 3) | SG90 Elbow Pitch Servo | Motion Output | `OUTPUT` (PWM) | 5V TTL |
| **D9** | Gripper Servo PWM (Axis 4) | SG90 Claw Clamp Servo | Motion Output | `OUTPUT` (PWM) | 5V TTL |
| **D13**| Heartbeat LED | Onboard SMD LED | System Status | `OUTPUT` | 5V TTL |
| **A4 (SDA)**| I2C Data Line | MPU6050 SDA Pin | IMU Telemetry | `INPUT_PULLUP` | 5V / 3.3V I2C Bus |
| **A5 (SCL)**| I2C Clock Line | MPU6050 SCL Pin | IMU Telemetry | `OUTPUT` (I2C) | 5V / 3.3V I2C Bus |

---

## 2. Power and Ground Rails

| Terminal | Destination | Voltage | Max Current | Function |
|:---|:---|:---:|:---:|:---|
| External 5V Power Supply (+) | Servo VCC Rail (Red Wires) | +5.0V DC | 3.0A | Dedicated high-current servo supply |
| External 5V Power Supply (-) | Common System GND | 0V (GND) | Return | High-current ground return |
| Arduino 5V Pin | MPU6050 VCC | +5.0V / 3.3V | 10 mA | Low-noise sensor logic power |
| Arduino GND | Common System GND | 0V (GND) | Return | Common reference ground bonding |
