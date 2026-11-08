# Smart Railway Track System - Circuit Schematic & Interconnect Diagram

## 1. Schematic Flow & Sensor Routing

```
                          +----------------------------+
                          |  7.4V Li-ion Battery Pack  |
                          +-------------+--------------+
                                        |
                        +---------------+---------------+
                        | (+) +7.4V                     | (-) GND
                        v                               v
               +-----------------+             +-----------------+
               | L298N 12V Screw |             | L298N GND Screw |
               +--------+--------+             +--------+--------+
                        |                               |
                        | (5V Regulated Out)            +------------+
                        v                                            | (Common GND)
               +-----------------+                                   v
               | L298N 5V Screw  |------------------------> +-----------------+
               +--------+--------+                          |   Arduino UNO   |
                        |                                   |     GND Pins    |
                        v                                   +--------+--------+
               +-----------------+                                   |
               |  Arduino 5V Pin |                                   |
               +--------+--------+                                   |
                        |                                            |
        +---------------+---------------+                            |
        |               |               |                            |
        v               v               v                            |
 +-------------+ +-------------+ +-------------+                     |
 | 2x IR Crack | | HC-SR04     | | Gate Servo  |                     |
 | Sensors VCC | | Sonar VCC   | | SG90 VCC    |                     |
 +-------------+ +-------------+ +-------------+                     |
        |               |               |                            |
        | (D2, D3)      | (D4, D5)      | (D6 PWM)                   |
        v               v               v                            |
 [Arduino D2,D3] [Arduino D4,D5] [Arduino D6]                        |
        |               |               |                            |
        |               |               |                            |
        +---------------+---------------+----------------------------+
```

## 2. Multi-Aspect Railway Signal Head & Warning Buzzer

```
Arduino D7 (Green Aspect)  ----[ 220Ω ]---->| (Green LED Anode) ----+
Arduino D8 (Yellow Aspect) ----[ 220Ω ]---->| (Yellow LED Anode) ---+
Arduino D9 (Red Aspect)    ----[ 220Ω ]---->| (Red LED Anode) ------+--> System GND
Arduino D10 (Alarm Buzzer) ----------------->| (+) Piezo Buzzer -----+
```
