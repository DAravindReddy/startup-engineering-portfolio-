# Smart Railway Track System - Installation & Troubleshooting Guide

## 1. Toolchain & Firmware Setup

### Desktop Unit Testing
Compile and run the standalone C++14 verification suite with GCC:
```bash
cd 01_EMBEDDED_SYSTEMS/smart_railway_system
g++ -Wall -Wextra -Werror -pedantic -std=c++14 -I source_code/include source_code/src/*.cpp tests/unit_tests/test_railway_fsm.cpp -o test_rail.exe
./test_rail.exe
```

### Flashing Arduino Firmware
1. Open `source_code/main/smart_railway_system.ino` in Arduino IDE.
2. Select Board: **Arduino Uno**.
3. Select Port: **COMx**.
4. Click **Upload** (`Ctrl+U`).

---

## 2. Troubleshooting Matrix

| Problem / Observation | Probable Root Cause | Verification Step | Remediation |
|:---|:---|:---|:---|
| System indicates persistent Red Aspect (Crack Danger) on solid rails | Standoff distance too high (> 8mm) or potentiometer sensitivity unadjusted | Measure gap between sensor face and rail crown; check sensor onboard LED | Position sensor $3\text{ mm} - 5\text{ mm}$ above rail; adjust onboard trimmer until LED illuminates reliably over solid steel |
| Barrier gate servo hums or chatters at end of travel | Target servo angle exceeds mechanical horn limits (e.g. driving against stop pin) | Check physical barrier arm movement when powered off | Adjust `GATE_ANGLE_OPEN` (90°) or `GATE_ANGLE_CLOSED` (0°) in `config.h` |
| Sonar false-alarms on track gravel / ballast | Sonar mounted too low, detecting ground ballast reflection | Check sonar tilt angle relative to track bed | Elevate sonar module to at least $60\text{ mm}$ above rail plane and tilt upwards $3^\circ$ |
| Inspection trolley motor does not stop upon Red Aspect | PWM or Direction pins miswired to L298N driver | Check voltage on pin D11 (ENA) during Red signal | Verify ENA is pulled to 0V during emergency halt in `smart_railway_system.ino` |
