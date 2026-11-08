# Fire Fighting Robot - Bill of Materials (BOM) & Hardware Specifications

## 1. Components Summary

| Component Item | Description / Model | Quantity | Operating Voltage | Peak Current | Role in System |
|:---|:---|:---:|:---:|:---:|:---|
| **Microcontroller Board** | Arduino UNO Rev3 (ATmega328P) | 1 | 5V DC (7-12V Vin) | 50 mA | Core FSM and PWM controller |
| **Infrared Flame Sensor Array**| LM393 3-Channel IR Flame Modules | 3 | 3.3V - 5.0V DC | 15 mA each | Detects 760nm - 1100nm infrared radiation |
| **Submersible DC Water Pump** | 3V-6V DC Mini Submersible Pump | 1 | 5V - 6V DC | 650 mA (Under load) | Dispenses water through nozzle onto fire |
| **5V Single-Channel Relay Module**| Optocoupler-Isolated 5V Relay | 1 | 5V Logic Coil | 70 mA | High-current switching for water pump |
| **Water Reservoir & Tubing** | 250ml Plastic Reservoir + 6mm Silicone Tube| 1 | Mechanical | N/A | Fluid storage and routing |
| **Dual H-Bridge Motor Driver** | L298N Module | 1 | 5V Logic, 7-12V Motor | 2.0A per bridge | Bidirectional motor drive & PWM speed |
| **DC Geared Motors** | TT Gear Motor (1:48 gear ratio) | 2 | 3V - 6V DC | 300 mA (under load)| Left & Right differential drive wheels |
| **Robot Chassis Kit** | 2WD Acrylic Platform + Castor Wheel | 1 | Mechanical | N/A | Base chassis & caster wheel |
| **Battery Power Source** | 2x 18650 Li-ion Batteries (7.4V, 2600mAh)| 1 | 7.4V Nominal | 4.0A continuous | Motor and system power supply |
| **Audible Alarm & Strobe** | 5V Active Buzzer / High-Intensity LED | 1 | 5V DC | 30 mA | Acoustic warning upon fire sighting |
| **Connecting Wires & Standoffs**| Jumper Wires (M-M, M-F, F-F) | 30 | N/A | N/A | Point-to-point interconnects |

---

## 2. Power Rail & Current Analysis

```
Total Battery Rail: 7.4V (2S Li-ion Battery Pack)
├── L298N Motor Driver 12V Terminal (Unregulated 7.4V Rail)
│   ├── Left TT Motor:  ~250 mA (Nominal) / 450 mA (Stall)
│   └── Right TT Motor: ~250 mA (Nominal) / 450 mA (Stall)
└── 5V Logic & Auxiliary Rail (via Step-Down Regulator)
    ├── Arduino UNO:            ~45 mA
    ├── 3x Flame Sensors:       ~45 mA total (15 mA each)
    ├── 5V Relay Module Coil:   ~70 mA
    ├── DC Water Pump:          ~650 mA (Active during 3.0s spray bursts)
    └── Alarm Buzzer:           ~30 mA

Quiescent Patrol Current:   ~640 mA
Peak Current (During Spray): ~1.35 A
```
