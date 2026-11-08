# Gesture Controlled Robotic Arm - Bill of Materials (BOM) & Hardware Specifications

## 1. Components Summary

| Component Item | Description / Model | Quantity | Operating Voltage | Peak Current | Role in System |
|:---|:---|:---:|:---:|:---:|:---|
| **Microcontroller Board** | Arduino UNO Rev3 or Nano (ATmega328P) | 1 | 5V DC (7-12V Vin) | 50 mA | Core Kinematics & PWM Controller |
| **6-Axis Motion Sensor (IMU)**| InvenSense MPU6050 (3-axis Accel + Gyro)| 1 | 3.3V - 5.0V DC | 4 mA | Hand gesture roll/pitch measurement |
| **High-Torque Metal Servos** | MG996R (Base & Shoulder Articulation) | 2 | 4.8V - 6.6V DC | 1.4 A (Stall) | High-load joints (Joint 1 & Joint 2) |
| **Micro Servos** | TowerPro SG90 (Elbow & End-Effector Gripper)| 2 | 4.8V - 6.0V DC | 250 mA (Stall) | Light-load forearm & clamp claw |
| **4-DOF Robotic Arm Kit** | Laser-Cut Acrylic / Aluminum Bracket Kit | 1 | Mechanical | N/A | Mechanical linkages and joint pivots |
| **External 5V Power Supply** | 5V 3.0A DC Regulated Power Adapter | 1 | 5.0V DC | 3.0 A continuous | Dedicated power rail for all 4 servos |
| **Grip Activation Button** | Tactile Pushbutton or Flex Bend Sensor | 1 | 5V Logic | < 1 mA | Clamps and releases end-effector claw |
| **Breadboard / PCA9685** | Terminal distribution board & Jumper Wires | 1 | N/A | N/A | Power distribution and signal bus |

---

## 2. Power Rail Isolation & Current Budget

```
External 5.0V 3A Regulated Supply Rail
├── Base MG996R Servo:     ~350 mA (Nominal) / 1.4 A (Peak Accel)
├── Shoulder MG996R Servo: ~450 mA (Nominal) / 1.5 A (Holding Torque)
├── Elbow SG90 Servo:       ~120 mA (Nominal) / 300 mA (Peak)
└── Gripper SG90 Servo:     ~80 mA (Holding Clamp) / 250 mA (Peak)

Peak Transient Servo Current: ~3.2 A (Simultaneous fast multi-axis slew)
Nominal Motion Current:       ~950 mA

Arduino UNO / Nano Logic Rail (USB or 7-12V Vin)
├── ATmega328P MCU:  ~45 mA
└── MPU6050 IMU:     ~4 mA
```
