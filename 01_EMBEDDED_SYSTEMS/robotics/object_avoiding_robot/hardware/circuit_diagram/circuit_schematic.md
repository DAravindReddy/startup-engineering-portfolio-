# Object Avoiding Robot - Circuit Schematic & Interconnect Diagram

## 1. Schematic Flow & Wiring Interconnects

```
                      +-----------------------------+
                      |   7.4V Li-ion Battery Pack  |
                      |   (2x 18650 in Series)      |
                      +--------------+--------------+
                                     |
                       +-------------+-------------+
                       | (+) +7.4V                 | (-) GND
                       v                           v
              +------------------+       +------------------+
              | L298N 12V Screw  |       |  L298N GND Screw |
              +------------------+       +--------+---------+
                       |                          |
                       | (78M05 Regulated)        +-------------+
                       v                                        | (Common GND)
              +------------------+                              v
              |  L298N 5V Screw  |-------------------> +------------------+
              +--------+---------+                     |   Arduino UNO    |
                       |                               |   GND Pins       |
                       v                               +--------+---------+
              +------------------+                              |
              | Arduino 5V Pin   |                              |
              +--------+---------+                              |
                       |                                        |
       +---------------+---------------+                        |
       |               |               |                        |
       v               v               v                        |
+-------------+ +-------------+ +-------------+                 |
| HC-SR04 VCC | |  SG90 VCC   | | Arduino VCC |                 |
+-------------+ +-------------+ +-------------+                 |
       |               |               |                        |
       | (Echo/Trig)   | (PWM D3)      |                        |
       v               v               v                        |
[Arduino D11,D12] [Arduino D3]   [Internal MCU]                 |
       |               |               |                        |
       | (GND)         | (GND)         | (GND)                  |
       +---------------+---------------+------------------------+
```

## 2. Differential Drive Motor Actuation

```
                  +---------------------------+
                  |    L298N Motor Driver     |
                  | OUT1 / OUT2   OUT3 / OUT4 |
                  +------+-------------+------+
                         |             |
                         v             v
                  +-------------+ +-------------+
                  | Left Motor  | | Right Motor |
                  | (TT Geared) | | (TT Geared) |
                  +-------------+ +-------------+
```
