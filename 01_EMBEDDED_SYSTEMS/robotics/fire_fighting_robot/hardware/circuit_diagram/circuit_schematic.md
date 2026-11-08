# Fire Fighting Robot - Circuit Schematic & Wiring Guide

## 1. Schematic Flow & Power Routing

```
                           +---------------------------+
                           |  7.4V Li-ion Battery Pack |
                           +-------------+-------------+
                                         |
                         +---------------+---------------+
                         | (+) +7.4V                     | (-) GND
                         v                               v
                +-----------------+             +-----------------+
                | L298N 12V Screw |             | L298N GND Screw |
                +--------+--------+             +--------+--------+
                         |                               |
                         | (Regulated 5V)                +------------+
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
  | 3x Flame    | | 5V Relay    | | Fire Buzzer |                     |
  | Sensors VCC | | Module VCC  | | Anode (+)   |                     |
  +-------------+ +-------------+ +-------------+                     |
         |               |               |                            |
         | (D2, D3, D4)  | (IN: D11)     | (Cathode)                  |
         v               v               v                            |
  [Arduino Inputs][Arduino D11]   [Internal MCU]                      |
         |               |               |                            |
         | (GND)         | (GND)         | (GND)                      |
         +---------------+---------------+----------------------------+
```

## 2. High-Current Extinguisher Pump Circuit

```
  +5.0V / 7.4V Supply Rail
         |
         v
+------------------+
| Relay Terminal   |
| COM (Common)     |
+--------+---------+
         |
         | [Relay Switch: Energized by D11]
         v
+------------------+
| Relay Terminal   |
| NO (Norm. Open)  |
+--------+---------+
         |
         v
  (+) Red Lead
+------------------+
| Submersible DC   |
| Water Pump       |
+--------+---------+
  (-) Black Lead
         |
         v
System Common GND
```
