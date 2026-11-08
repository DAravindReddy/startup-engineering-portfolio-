# Arduino Nano 33 BLE Sense AI - Functional Requirements & Software Design

## 1. Functional Requirements

- **FR-01 (100 Hz Vibration Ingestion)**: The system shall sample the onboard 3-axis accelerometer at 100 Hz, computing vector magnitude with sub-millisecond precision.
- **FR-02 (Rolling Window Feature Extraction)**: The system shall buffer 64 continuous samples and calculate vibration RMS, peak-to-peak amplitude, and crest factor.
- **FR-03 (Thermal Trend Extraction)**: The system shall sample the HTS221 temperature sensor at 1 Hz and compute the first-order thermal derivative ($\Delta T / \Delta t$).
- **FR-04 (On-Device TinyML Anomaly Inference)**: The system shall evaluate the multi-variable feature vector at 2 Hz, outputting a continuous 0.0% to 100.0% Anomaly Score.
- **FR-05 (BLE GATT Wireless Telemetry)**: The system shall advertise as a BLE peripheral and send push notifications to connected clients whenever values update.
- **FR-06 (Multi-Tier State Classification)**:
  - Score < 50.0% $\rightarrow$ `MACHINE_STATE_NORMAL`
  - 50.0% $\le$ Score < 75.0% $\rightarrow$ `MACHINE_STATE_WARNING`
  - Score $\ge$ 75.0% $\rightarrow$ `MACHINE_STATE_CRITICAL_FAULT`

---

## 2. Software Design & Implementation Architecture

```text
source_code/
├── include/
│   ├── config.h                   # BLE UUIDs, sampling rates, fault thresholds
│   ├── sensor_manager.h           # LSM9DS1 and HTS221 driver wrappers
│   ├── feature_extractor.h        # 64-sample ring buffer & statistical calculator
│   ├── tinyml_anomaly_detector.h  # Mahalanobis distance & continuous scoring
│   └── ble_telemetry_service.h    # ArduinoBLE GATT service & notification handlers
└── src/
    ├── sensor_manager.cpp
    ├── feature_extractor.cpp
    ├── tinyml_anomaly_detector.cpp
    └── ble_telemetry_service.cpp
```
