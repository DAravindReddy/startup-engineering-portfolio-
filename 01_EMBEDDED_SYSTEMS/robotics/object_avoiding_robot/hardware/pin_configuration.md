# Object Avoiding Robot - Pin Configuration & Signal Mapping

## 1. Arduino UNO Microcontroller Pin Assignments

| Arduino Pin | Signal Function | Target Peripheral | Subsystem | Pin Mode | Logic Level |
|:---:|:---|:---|:---|:---:|:---:|
| **D3** | PWM Servo Control | SG90 Orange Signal Wire | Ultrasonic Turret | `OUTPUT` (PWM) | 5V TTL |
| **D4** | Optional Buzzer | Active Buzzer (+) | Alert Subsystem | `OUTPUT` | 5V TTL |
| **D5** | Motor Left ENA | L298N ENA (Enable A) | Motor Left PWM | `OUTPUT` (PWM) | 5V TTL |
| **D6** | Motor Left IN1 | L298N IN1 | Left Motor Fwd | `OUTPUT` | 5V TTL |
| **D7** | Motor Left IN2 | L298N IN2 | Left Motor Rev | `OUTPUT` | 5V TTL |
| **D8** | Motor Right IN3 | L298N IN3 | Right Motor Fwd | `OUTPUT` | 5V TTL |
| **D9** | Motor Right IN4 | L298N IN4 | Right Motor Rev | `OUTPUT` | 5V TTL |
| **D10**| Motor Right ENB | L298N ENB (Enable B) | Motor Right PWM | `OUTPUT` (PWM) | 5V TTL |
| **D11**| Sonar Trigger | HC-SR04 Trig Pin | Sonar Transmitter | `OUTPUT` | 5V TTL |
| **D12**| Sonar Echo | HC-SR04 Echo Pin | Sonar Receiver | `INPUT` | 5V TTL |
| **D13**| Heartbeat LED | Onboard SMD LED | System Status | `OUTPUT` | 5V TTL |

---

## 2. Power and Ground Rails

| Source / Terminal | Destination | Voltage | Max Current | Function |
|:---|:---|:---:|:---:|:---|
| Battery Pack (+) | L298N 12V Screw Terminal | +7.4V | 3.0A | Motor supply rail |
| Battery Pack (-) | L298N GND Terminal | 0V (GND) | Return | System ground reference |
| L298N GND Terminal | Arduino GND | 0V (GND) | Return | Common ground bonding |
| L298N 5V Out | Arduino 5V Pin | +5.0V | 1.0A | Logic power supply |
| Arduino 5V Pin | HC-SR04 VCC & SG90 Red | +5.0V | 350 mA | Sensor & Servo power rail |
