# Gesture Controlled Robotic Arm - Physical Hardware Connections & Assembly Guide

## 1. Mechanical Assembly Sequence

1. **Base Turntable (Joint 1)**: Mount the high-torque MG996R servo into the base chassis plate pointing vertically upwards. Fasten the rotating platform to the servo horn.
2. **Shoulder Pivot (Joint 2)**: Mount the second MG996R servo horizontally onto the rotating base bracket. Attach the lower humerus linkage arm to its horn.
3. **Elbow Joint (Joint 3)**: Mount the SG90 micro-servo at the elbow pivot between the lower arm and forearm linkage.
4. **End-Effector Gripper (Joint 4)**: Mount the SG90 micro-servo onto the claw mechanism. Link the servo horn to the driving gear of the opposing fingers.
5. **Gesture Glove Assembly**: Fasten the MPU6050 breakout board onto the dorsal surface (back) of a lightweight glove using velcro or nylon ties. Mount the grip trigger pushbutton along the index finger tip.

---

## 2. Electrical Wiring Checklist

- [ ] **MPU6050 IMU to Arduino**:
  - `VCC` $\rightarrow$ Arduino `5V` (or `3.3V` depending on breakout LDO)
  - `GND` $\rightarrow$ Arduino `GND`
  - `SDA` $\rightarrow$ Arduino `A4`
  - `SCL` $\rightarrow$ Arduino `A5`
- [ ] **Grip Trigger Button**:
  - Terminal 1 $\rightarrow$ Arduino `D2`
  - Terminal 2 $\rightarrow$ Arduino `GND` (Internal pull-up enabled in software)
- [ ] **Servo Control Lines to Arduino**:
  - Base Servo Signal (Yellow/Orange) $\rightarrow$ Arduino `D3` (PWM)
  - Shoulder Servo Signal (Yellow/Orange) $\rightarrow$ Arduino `D5` (PWM)
  - Elbow Servo Signal (Yellow/Orange) $\rightarrow$ Arduino `D6` (PWM)
  - Gripper Servo Signal (Yellow/Orange) $\rightarrow$ Arduino `D9` (PWM)
- [ ] **Servo Power Lines (External 5V 3A Supply)**:
  - All 4 Servo Red (VCC) Wires $\rightarrow$ External 5V Power Supply (+)
  - All 4 Servo Brown/Black (GND) Wires $\rightarrow$ External 5V Power Supply (-)
  - External Supply (-) $\rightarrow$ Arduino `GND` (Common Ground Bond)
