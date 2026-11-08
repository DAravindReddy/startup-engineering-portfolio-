# Gesture Controlled Robotic Arm - Installation & Troubleshooting Guide

## 1. Toolchain & Firmware Setup

### Desktop Unit Testing
Execute the standalone C++14 unit test harness with GCC:
```bash
cd 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm
g++ -Wall -Wextra -Werror -pedantic -std=c++14 -I source_code/include source_code/src/*.cpp tests/unit_tests/test_kinematics_mapper.cpp -o test_arm.exe
./test_arm.exe
```

### Flashing Arduino Firmware
1. Connect Arduino UNO / Nano via USB.
2. Open `source_code/main/gesture_controlled_robotic_arm.ino` in Arduino IDE.
3. Select Board: **Arduino Uno** (or **Arduino Nano**).
4. Select Port: **COMx**.
5. Click **Upload** (`Ctrl+U`).

---

## 2. Troubleshooting Matrix

| Problem / Observation | Probable Root Cause | Verification Step | Remediation |
|:---|:---|:---|:---|
| Console reports `MPU6050 IMU connection failed` | I2C wiring disconnected, wrong address, or missing pull-up resistors | Verify wiring: SDA to A4, SCL to A5. Check if AD0 pin is tied to VCC (0x69) or GND (0x68) | Reconnect SDA/SCL wires; tie AD0 to GND for default 0x68 address |
| Servos vibrate and twitch continuously | Shared power rail with Arduino, or missing common ground bond | Check voltage on servo VCC rail with multimeter during motion | Power servos via independent 5V 3A adapter; bond supply GND to Arduino GND |
| Arm jerks violently when hand makes sudden movement | Slew-rate velocity limit bypassed or set too high | Inspect `config.h` for `SERVO_SLEW_RATE_DEG_PER_SEC` | Verify slew rate is active and set between $60^\circ/\text{s} - 90^\circ/\text{s}$ |
| Gripper servo buzzes loudly when closed | Mechanical interference preventing servo from reaching target angle | Inspect claw jaws when clamping an object | Adjust `GRIPPER_CLOSED_ANGLE` in `config.h` so servo motor does not stall against rigid objects |
