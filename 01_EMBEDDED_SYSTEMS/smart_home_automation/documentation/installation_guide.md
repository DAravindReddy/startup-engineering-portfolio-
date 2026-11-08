# Installation and Flashing Guide

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Toolchain Prerequisites
- **Arduino IDE 2.x** or **Arduino CLI** (v0.35+).
- **AVR Core Support:** Install `arduino:avr` core (for ATmega328P / Arduino UNO).
- **External Libraries:**
  - `Servo` (Built into Arduino AVR core).
  - `SoftwareSerial` (Built into Arduino AVR core).
- **Host PC Tools (Optional for Testing & Scripts):**
  - Python 3.10+
  - `pip install pyserial` (for `scripts/mock_bluetooth_client.py`).
  - GCC / MinGW-w64 (for native unit test compilation).

---

## 2. Flashing via Arduino IDE

1. Open Arduino IDE.
2. Open `01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/main.ino`.
3. Select **Board**: `Arduino Uno` from `Tools -> Board -> Arduino AVR Boards -> Arduino Uno`.
4. Select **Port**: The corresponding COM port (e.g., `COM3` on Windows, `/dev/ttyACM0` on Linux).
5. Click **Verify** (Checkmark) to compile firmware.
6. Click **Upload** (Right Arrow) to flash program memory.
7. Open **Serial Monitor** at **9600 Baud** to observe system initialization.

---

## 3. Flashing via Arduino CLI

```bash
# Compile Sketch
arduino-cli compile --fqbn arduino:avr:uno 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src

# Upload Sketch (replace COM3 with your target port)
arduino-cli upload -p COM3 --fqbn arduino:avr:uno 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src
```

---

## 4. Running Native Unit Tests (Workstation Host)

You can verify the core control logic on your development workstation using GCC without flashing physical hardware:

```bash
cd 01_EMBEDDED_SYSTEMS/smart_home_automation/tests/unit_tests
g++ -Wall -Wextra -std=c++14 \
    ../../source_code/src/bluetooth_handler.cpp \
    ../../source_code/src/sensor_manager.cpp \
    ../../source_code/src/actuator_controller.cpp \
    ../../source_code/src/irrigation_system.cpp \
    test_smart_home.cpp -I../../source_code/include -o test_runner
./test_runner
```
