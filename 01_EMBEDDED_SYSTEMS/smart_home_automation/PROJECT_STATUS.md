# Project Status: Smart Home Automation with Bluetooth

- **Project Name:** `smart_home_automation`
- **Category:** `01_EMBEDDED_SYSTEMS`
- **Development Status:** `Prototype / Testing`
- **Current Version:** `v1.1.0`
- **Lead Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))

---

## 1. Requirements & Technologies

* **Hardware Platform:**
  - Microchip ATmega328P (Arduino UNO Rev3)
  - HC-05 Bluetooth Transceiver Module (SPP / UART)
  - DHT11 Temperature & Relative Humidity Sensor
  - Resistive Soil Moisture Sensor with LM393 Comparator
  - L298N Dual H-Bridge Motor Driver
  - 12V Brushed DC Motor
  - SG90 9g Micro Servo Motor
  - 1-Channel Optocoupled 5V Relay Module
  - 5V/12V Submersible DC Water Pump
  - 5mm Status & Appliance LEDs
* **Software Toolchains & Libraries:**
  - Arduino IDE 2.x / Arduino CLI (`arduino:avr` core)
  - GCC 16.1+ / Clang (for native workstation unit tests)
  - Python 3.10+ (`pyserial` for automated test client)
* **Core Technologies:**
  - Embedded C++14
  - Non-blocking cooperative time-sliced scheduler (`millis()`)
  - Finite State Machine with Hysteresis (FSM)
  - Hardware Safety Watchdog Supervisor
  - Structured RFC-compliant JSON telemetry streaming

---

## 2. Verification Status

| Component | Status | Notes |
|---|:---:|---|
| Bluetooth Command Lexical Tokenizer | :white_check_mark: Verified | 100% passed in native C++ unit tests (`test_smart_home.cpp`). |
| Soil Moisture Normalization & Calibration | :white_check_mark: Verified | Integer mapping verified across boundaries (0% air, 100% water). |
| Irrigation Hysteresis State Machine | :white_check_mark: Verified | Clean state transitions without boundary hunting. |
| Safety Watchdog Auto-Cutoff (15s) | :white_check_mark: Verified | Trips reliably after continuous 15-second operation. |
| Physical Hardware Pin Budgeting | :white_check_mark: Verified | Level shifter and flyback protection documented. |

---

## 3. Known Limitations

1. **Sensor Probe Degradation:** Standard resistive probes experience electrolysis when DC voltage is applied continuously. 
2. **Range Boundary:** Bluetooth Classic (HC-05) operates within 10 meters line-of-sight.
3. **Silicon Flash Ceiling:** ATmega328P has 32 KB Flash; current build consumes 25.5%, leaving modest room for additional features.

---

## 4. Next Development Milestones

- [ ] **Milestone 1:** Port codebase to ESP32-C3 / ESP32-S3 supporting Matter / Thread protocol.
- [ ] **Milestone 2:** Replace resistive probe with I2C capacitive soil moisture sensor with temperature compensation.
- [ ] **Milestone 3:** Deploy Edge ML evapotranspiration model to forecast watering needs based on ambient VPD.
