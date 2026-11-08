# Object Avoiding Robot - Bill of Materials (BOM) & Hardware Specifications

## 1. Components Summary

| Component Item | Description / Model | Quantity | Operating Voltage | Peak Current | Role in System |
|:---|:---|:---:|:---:|:---:|:---|
| **Microcontroller Board** | Arduino UNO Rev3 (ATmega328P) | 1 | 5V DC (7-12V Vin) | 50 mA | Core FSM and PWM controller |
| **Ultrasonic Distance Sensor** | HC-SR04 | 1 | 5V DC | 15 mA | Contactless obstacle detection (2cm - 400cm) |
| **Micro Servo Motor** | TowerPro SG90 (180° rotation) | 1 | 4.8V - 6.0V DC | 250 mA (Stall) | Dynamic ultrasonic panning turret |
| **Dual H-Bridge Motor Driver** | L298N Module | 1 | 5V Logic, 7-12V Motor | 2.0A per bridge | Bidirectional DC motor drive & PWM speed |
| **DC Geared Motors** | TT Gear Motor (1:48 gear ratio) | 2 | 3V - 6V DC | 300 mA (under load)| Left & Right differential drive wheels |
| **Robot Chassis Kit** | 2WD Acrylic Platform + Castor Wheel | 1 | Mechanical | N/A | Chassis, motor mounts, free caster wheel |
| **Battery Power Source** | 2x 18650 Li-ion Batteries (7.4V, 2600mAh)| 1 | 7.4V Nominal | 4.0A continuous | Motor and system power supply |
| **Buck Converter / Regulator** | LM2596 DC-DC Step-Down (Optional) | 1 | In: 7-12V, Out: 5V | 2.0A | Stable 5V logic rail for Arduino and Servo |
| **Connecting Wires** | Jumper Wires (M-M, M-F, F-F) | 25 | N/A | N/A | Point-to-point interconnects |

---

## 2. Power Distribution & Current Budget

```
Total Battery Rail: 7.4V (2S Li-ion Battery Pack)
├── L298N Motor Driver 12V Pin (Direct 7.4V Unregulated)
│   ├── Left TT Motor:  ~250 mA (Nominal) / 450 mA (Stall)
│   └── Right TT Motor: ~250 mA (Nominal) / 450 mA (Stall)
└── 5V Logic Bus (via L298N onboard 78M05 or LM2596 Regulator)
    ├── Arduino UNO:     ~45 mA
    ├── HC-SR04 Sonar:   ~15 mA
    └── SG90 Servo:      ~100 mA (Moving) / 350 mA (Peak)

Peak Transient System Current: ~1.45 Amperes
Nominal Cruising Current:       ~660 Milliamperes
Estimated Battery Life (2600mAh Pack): ~3.8 Hours continuous autonomous cruise
```
