# Arduino Nano 33 BLE Sense AI - Installation & Troubleshooting Guide

## 1. Toolchain & Firmware Setup

### Desktop Unit Testing
Execute the standalone C++14 unit test harness with GCC:
```bash
cd 01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai
g++ -Wall -Wextra -Werror -pedantic -std=c++14 -I source_code/include source_code/src/*.cpp tests/unit_tests/test_tinyml_pipeline.cpp -o test_tinyml.exe
./test_tinyml.exe
```

### Flashing Arduino Firmware
1. In Arduino IDE Library Manager, install:
   - `Arduino_LSM9DS1` (or `Arduino_BMI270_BMM150` for Rev2)
   - `Arduino_HTS221`
   - `ArduinoBLE`
2. Open `source_code/main/arduino_ble_sense_ai.ino`.
3. Select Board: **Arduino Nano 33 BLE** (under Arduino Mbed OS Nano Boards).
4. Select Port: **COMx**.
5. Click **Upload** (`Ctrl+U`).

---

## 2. Troubleshooting Matrix

| Problem / Observation | Probable Root Cause | Verification Step | Remediation |
|:---|:---|:---|:---|
| BLE device `KW-EdgeHealth-01` does not appear in scanner apps | `BLE.begin()` failed or Bluetooth permission blocked on phone | Check serial monitor for `[INIT] TinyML engine initialized` | Power-cycle board; ensure mobile phone Bluetooth and Location permissions are enabled |
| Anomaly score fluctuates erratically on stationary board | Sensor sampling uncalibrated or high acoustic floor | Observe raw acceleration values in serial monitor | Ensure board is securely fastened; gravity baseline should read $\sim 9.81\text{ m/s}^2$ |
| Firmware fails to compile on Arduino IDE | Nano 33 BLE board package or libraries missing | Check Arduino Board Manager for `Arduino Mbed OS Nano Boards` | Install Mbed OS Nano board package via Board Manager |
| Temperature reads unrealistically high (> 45°C) at room temp | Internal board heating from nRF52840 regulator warming HTS221 | Compare sensor reading to thermal probe on casing | Apply $-3.5^\circ\text{C}$ calibration offset in `sensor_manager.cpp` to account for PCB self-heating |
