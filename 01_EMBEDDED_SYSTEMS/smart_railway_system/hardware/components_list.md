# Smart Railway Track System - Bill of Materials (BOM) & Hardware Specifications

## 1. Components Summary

| Component Item | Description / Model | Quantity | Operating Voltage | Peak Current | Role in System |
|:---|:---|:---:|:---:|:---:|:---|
| **Microcontroller Board** | Arduino UNO Rev3 (ATmega328P) | 1 | 5V DC (7-12V Vin) | 50 mA | Core FSM and Safety Interlock |
| **Optical Rail Crack Sensors**| TCRT5000 / LM393 IR Reflective Modules | 2 | 3.3V - 5.0V DC | 20 mA each | Dual-rail fracture & fissure detection |
| **Ultrasonic Distance Sensor** | HC-SR04 | 1 | 5.0V DC | 15 mA | Long-range track obstacle perception (2-400cm)|
| **Barrier Gate Servo** | TowerPro SG90 (90° level-crossing arm) | 1 | 4.8V - 6.0V DC | 250 mA (Stall) | Automated road-crossing barrier arm |
| **Color Light Signal LEDs** | 5mm Diffused LEDs (Red, Yellow, Green)| 3 | 2.0V - 3.2V (w/ 220Ω)| 15 mA each | Multi-aspect railway track signal head |
| **Acoustic Warning Alarm** | 5V Active Piezo Buzzer | 1 | 5.0V DC | 30 mA | Audible danger alert for workers & pedestrians|
| **Inspection Trolley Motor** | TT Geared DC Motor (1:48) | 1 | 4.5V - 6.0V DC | 300 mA (under load)| Autonomous propulsion along test rails |
| **Motor Driver Board** | L298N or L293D Mini Driver Module | 1 | 5V Logic, 7-12V Motor | 1.0 A | Forward drive & emergency braking of trolley |
| **Power Supply** | 7.4V Li-ion Battery (2x 18650) or 9V Adapter| 1 | 7.4V - 9.0V DC | 2.0 A continuous | Complete system power source |
| **Current-Limiting Resistors** | 220Ω 1/4W Metal Film Resistors | 3 | N/A | N/A | Current limits for Green, Yellow, Red LEDs |

---

## 2. Power Consumption Budget

```
Total Battery Rail: 7.4V (2S Li-ion Battery Pack)
├── Inspection Trolley Motor (via L298N): ~200 mA (Nominal) / 350 mA (Stall)
└── 5V Regulated Logic Bus
    ├── Arduino UNO Core:              ~45 mA
    ├── 2x IR Rail Sensors (TCRT5000):  ~40 mA total
    ├── HC-SR04 Sonar:                 ~15 mA
    ├── SG90 Gate Servo:               ~80 mA (Moving) / 250 mA (Holding)
    ├── Active Signal Aspect LED:       ~15 mA
    └── Alert Buzzer:                  ~30 mA

Quiescent Nominal Current (Green Aspect): ~310 mA
Peak Current (Emergency Halt / Gate Moving): ~750 mA
```
