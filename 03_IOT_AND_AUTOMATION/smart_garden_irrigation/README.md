# Smart Garden Irrigation & Microclimate Compensation System

**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Organization:** Kernelwise Labs  
**Domain:** IoT, Smart Agriculture, Embedded Firmware, Power Actuation  

---

## 1. Project Overview

The **Smart Garden Irrigation System** is an industrial-grade autonomous irrigation controller built for ESP32 and Arduino platforms. It replaces error-prone open-loop timers with dynamic closed-loop telemetry:
- **Corrosion-Resistant Sensing:** Capacitive dielectric soil moisture measurement with real-time Exponential Moving Average (EMA) filtering.
- **Dynamic Climate Compensation:** Automatically adjusts moisture irrigation thresholds based on ambient temperature and relative humidity vapor deficit.
- **Safe Relay Actuation:** Hysteresis-driven Finite State Machine (FSM) enforcing a $15.0\text{s}$ maximum single run duration and a mandatory $10.0\text{s}$ soaking cooldown.
- **Remote Telemetry & Control:** Stream JSON telemetry over MQTT and accept remote manual overrides with safety fallback timers.

---

## 2. Directory Architecture

```
smart_garden_irrigation/
│
├── README.md
├── PROJECT_STATUS.md
├── LICENSE
├── .gitignore
│
├── source_code/
│   ├── include/
│   │   └── irrigation_controller.h
│   ├── src/
│   │   └── irrigation_controller.c
│   └── main/
│       └── smart_garden_irrigation.ino
│
├── hardware/
│   ├── components_list.md
│   ├── pin_configuration.md
│   └── hardware_connections.md
│
├── architecture/
│   ├── system_architecture.md
│   └── block_diagram.md
│
├── documentation/
│   └── technical_documentation.md
│
├── whitepaper/
│   └── whitepaper.md
│
└── tests/
    └── test_irrigation_controller.cpp
```

---

## 3. Quickstart & Verification

### Compile & Run Formal Test Suite
```bash
g++ -Wall -Wextra -Werror -pedantic -std=c++14 \
    -I source_code/include \
    source_code/src/irrigation_controller.c \
    tests/test_irrigation_controller.cpp \
    -o test_runner
./test_runner
```
*Expected Output:*
```
[PASS] test_initialization
[PASS] test_sensor_ema_filtering
[PASS] test_environmental_adaptation
[PASS] test_hysteresis_and_cooldown_cycle
[PASS] test_max_duration_and_safety_lockout
ALL UNIT TESTS COMPLETED SUCCESSFULLY (100%)
```
