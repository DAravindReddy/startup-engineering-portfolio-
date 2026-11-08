# Smart Garden Irrigation - Pinout & Electrical Interfacing

**Organization:** Kernelwise Labs  

---

## 1. ESP32 Pin Mapping Table

| Peripheral | Component Pin | ESP32 GPIO | Electrical Standard | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **Capacitive Soil Sensor** | Analog VOUT | GPIO 34 (ADC1_CH6) | 0.0V – 3.3V Analog | Low-noise ADC1 channel (WiFi safe) |
| | VCC | 3V3 | Power Bus | Clean regulated 3.3V rail |
| | GND | GND | Ground | Common star ground point |
| **DHT22 Sensor** | Data Pin | GPIO 4 | 3.3V Digital (1-Wire) | 4.7kΩ pull-up to 3V3 required |
| | VCC | 3V3 | Power Bus | Filtered 3.3V rail |
| | GND | GND | Ground | Common ground |
| **5V Relay Module** | IN (Trigger) | GPIO 23 | 3.3V Logic Output | Active-LOW optocoupled input |
| | VCC | 5V | Power Bus | Sourced from LM2596 5V output |
| | GND | GND | Ground | Optocoupler return ground |
| **Heartbeat LED** | Anode (+) | GPIO 2 | 3.3V Logic Output | Current limited via 330Ω |
| | Cathode (-) | GND | Ground | Onboard blue status LED |

---

## 2. Power Rail Segmentation

- **12V High-Current Rail:** Direct from SMPS to Relay Common (`COM`) terminal and Pump negative ground.
- **5V Logic Rail:** From LM2596 Buck Converter to ESP32 `VIN` and Relay coil `VCC`.
- **3.3V Analog Rail:** From ESP32 onboard LDO to sensors, ensuring zero inductive noise coupling from the motor.
