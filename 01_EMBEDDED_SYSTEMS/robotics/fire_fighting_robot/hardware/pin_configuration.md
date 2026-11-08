# Fire Fighting Robot - Pin Configuration & Signal Mapping

## 1. Arduino UNO Microcontroller Pinout

| Arduino Pin | Signal Function | Target Peripheral | Subsystem | Pin Mode | Logic Level |
|:---:|:---|:---|:---|:---:|:---:|
| **D2** | Left Flame Sensor (DO) | Left IR Flame Module | Flame Perception | `INPUT_PULLUP` | 5V TTL (Active-LOW) |
| **D3** | Center Flame Sensor (DO)| Center IR Flame Module | Flame Perception | `INPUT_PULLUP` | 5V TTL (Active-LOW) |
| **D4** | Right Flame Sensor (DO)| Right IR Flame Module | Flame Perception | `INPUT_PULLUP` | 5V TTL (Active-LOW) |
| **D5** | Motor Left ENA | L298N ENA (PWM) | Left Motor Speed | `OUTPUT` (PWM) | 5V TTL |
| **D6** | Motor Left IN1 | L298N IN1 | Left Motor Fwd | `OUTPUT` | 5V TTL |
| **D7** | Motor Left IN2 | L298N IN2 | Left Motor Rev | `OUTPUT` | 5V TTL |
| **D8** | Motor Right IN3 | L298N IN3 | Right Motor Fwd | `OUTPUT` | 5V TTL |
| **D9** | Motor Right IN4 | L298N IN4 | Right Motor Rev | `OUTPUT` | 5V TTL |
| **D10**| Motor Right ENB | L298N ENB (PWM) | Right Motor Speed| `OUTPUT` (PWM) | 5V TTL |
| **D11**| Water Pump Relay (IN) | 5V Relay Module | Water Suppression| `OUTPUT` | 5V TTL (Active-HIGH) |
| **D12**| Optional Nozzle Servo | SG90 Nozzle Mount | Spray Sweeper | `OUTPUT` (PWM) | 5V TTL |
| **D13**| Fire Alarm Buzzer / Strobe| 5V Active Buzzer | Acoustic Alert | `OUTPUT` | 5V TTL |

---

## 2. Power and Ground Rails

| Source / Terminal | Destination | Voltage | Max Current | Function |
|:---|:---|:---:|:---:|:---|
| Battery Pack (+) | L298N 12V Screw Terminal | +7.4V | 3.0A | Motor power rail |
| Battery Pack (-) | L298N GND Screw Terminal | 0V (GND) | Return | Main system ground reference |
| L298N GND | Arduino GND | 0V (GND) | Return | Common ground reference bonding |
| L298N 5V Out | Arduino 5V Pin | +5.0V | 1.0A | Logic power rail |
| Relay Common (COM)| Battery Pack (+) or 5V | +5.0V / 7.4V | 1.5A | Pump power rail |
| Relay Normally Open (NO)| DC Water Pump (+) | Switched VCC | 800 mA | Switched pump supply |
| DC Water Pump (-) | System GND | 0V (GND) | Return | Pump ground return |
