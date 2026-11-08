# Smart Railway Track System - Pin Configuration & Signal Mapping

## 1. Arduino UNO Microcontroller Pinout

| Arduino Pin | Signal Function | Target Peripheral | Subsystem | Pin Mode | Logic Level |
|:---:|:---|:---|:---|:---:|:---:|
| **D2** | Left Rail Crack Sensor | Left TCRT5000 IR Sensor | Rail Integrity | `INPUT_PULLUP` | 5V TTL (Active-HIGH on crack) |
| **D3** | Right Rail Crack Sensor| Right TCRT5000 IR Sensor | Rail Integrity | `INPUT_PULLUP` | 5V TTL (Active-HIGH on crack) |
| **D4** | Sonar Trigger Pulse | HC-SR04 Trig Pin | Obstacle Perception | `OUTPUT` | 5V TTL |
| **D5** | Sonar Echo Width | HC-SR04 Echo Pin | Obstacle Perception | `INPUT` | 5V TTL |
| **D6** | Crossing Gate Servo PWM| SG90 Barrier Arm Servo | Level Crossing Gate | `OUTPUT` (PWM) | 5V TTL |
| **D7** | Signal Green LED | Green Signal Aspect (Clear)| Railway Signaling | `OUTPUT` | 5V TTL (via 220Ω) |
| **D8** | Signal Yellow LED | Yellow Signal Aspect (Warn)| Railway Signaling | `OUTPUT` | 5V TTL (via 220Ω) |
| **D9** | Signal Red LED | Red Signal Aspect (Danger) | Railway Signaling | `OUTPUT` | 5V TTL (via 220Ω) |
| **D10**| Acoustic Alarm Buzzer | 5V Active Piezo Buzzer | Alert Subsystem | `OUTPUT` | 5V TTL |
| **D11**| Trolley Motor ENA | L298N ENA (PWM) | Trolley Speed | `OUTPUT` (PWM) | 5V TTL |
| **D12**| Trolley Motor IN1 | L298N IN1 | Trolley Forward | `OUTPUT` | 5V TTL |
| **D13**| Trolley Motor IN2 | L298N IN2 | Trolley Reverse | `OUTPUT` | 5V TTL |

---

## 2. Power and Ground Rails

| Source / Terminal | Destination | Voltage | Max Current | Function |
|:---|:---|:---:|:---:|:---|
| Battery Pack (+) | L298N 12V Screw Terminal | +7.4V | 2.0A | Motor power rail |
| Battery Pack (-) | L298N GND Screw Terminal | 0V (GND) | Return | Main system ground reference |
| L298N 5V Out | Arduino 5V Pin | +5.0V | 1.0A | Logic power rail |
| Arduino 5V Pin | Sensors & Servo VCC | +5.0V | 400 mA | Regulated logic & sensor supply |
| Arduino GND | Common System GND | 0V (GND) | Return | Common reference ground bonding |
