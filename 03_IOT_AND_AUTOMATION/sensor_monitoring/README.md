# Multi-Sensor IoT Telemetry & Environmental Monitoring Node

**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Organization:** Kernelwise Labs  
**Domain:** IoT, Environmental Telemetry, Embedded DSP & Statistics  

---

## 1. Project Overview

The **Multi-Sensor Monitoring Node** is an industrial-grade environmental monitoring platform for ESP32 microcontrollers. It captures, filters, aggregates, and transmits real-time telemetry across 5 physical environmental dimensions:
- **Temperature & Humidity:** High-accuracy ambient climate monitoring via DHT22.
- **Barometric Pressure & Altitude:** Precision I2C atmospheric sensing via BMP280.
- **Illuminance:** Optical ambient light sensing via GL5528 photoresistor.
- **Air Quality & Hazardous Gases:** Detecting VOCs, smoke, and combustible gases via MQ-135.
- **On-Chip Statistical Aggregation:** Real-time computation of sample mean, min, max, and standard deviation over an in-memory 32-frame circular ring buffer.
- **Hierarchical Safety Alerts:** Warning and critical bitmask alert classification with sub-millisecond JSON MQTT publishing.

---

## 2. Directory Architecture

```
sensor_monitoring/
│
├── README.md
├── PROJECT_STATUS.md
├── LICENSE
├── .gitignore
│
├── source_code/
│   ├── include/
│   │   └── sensor_monitor.h
│   ├── src/
│   │   └── sensor_monitor.c
│   └── main/
│       └── sensor_monitoring.ino
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
    └── test_sensor_monitoring.cpp
```

---

## 3. Quickstart & Verification

### Compile & Run Formal Test Suite
```bash
g++ -Wall -Wextra -Werror -pedantic -std=c++14 \
    -I source_code/include \
    source_code/src/sensor_monitor.c \
    tests/test_sensor_monitoring.cpp \
    -o test_runner
./test_runner
```
*Expected Output:*
```
[PASS] test_initialization
[PASS] test_ring_buffer_and_stats
[PASS] test_alert_thresholds
[PASS] test_json_serialization
ALL UNIT TESTS COMPLETED SUCCESSFULLY (100%)
```
