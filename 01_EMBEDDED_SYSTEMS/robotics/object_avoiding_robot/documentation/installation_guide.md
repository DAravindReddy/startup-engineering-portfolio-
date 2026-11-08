# Object Avoiding Robot - Installation & Troubleshooting Guide

## 1. Toolchain & Firmware Flashing

### Requirements
- Arduino IDE 2.x or `arduino-cli`
- Target Board: **Arduino Uno** (ATmega328P)
- Standard library: `Servo.h` (built into Arduino Core)

### Uploading via Arduino IDE
1. Open `source_code/main/object_avoiding_robot.ino` in Arduino IDE.
2. Ensure header and source files (`config.h`, `motor_driver.h`, etc.) are located in the sketch folder or included via standard include paths.
3. Select Board: **Tools > Board > Arduino AVR Boards > Arduino Uno**.
4. Select Port: **Tools > Port > COMx (Arduino Uno)**.
5. Click **Upload** (`Ctrl+U`).

### Running Desktop Unit Tests
```bash
cd 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot
g++ -Wall -Wextra -Werror -pedantic -std=c++14 -I source_code/include source_code/src/*.cpp tests/unit_tests/test_navigation_fsm.cpp -o tests/unit_tests/test_fsm.exe
./tests/unit_tests/test_fsm.exe
```

---

## 2. Troubleshooting Matrix

| Problem / Observation | Probable Root Cause | Verification Step | Remediation |
|:---|:---|:---|:---|
| Robot turns in place instead of driving straight | One motor is wired with inverted polarity | Observe individual wheel rotations when held off the ground | Invert the motor leads at `OUT1/OUT2` or `OUT3/OUT4` screw terminals |
| Ultrasonic sensor reads 400cm / timeout constantly | `TRIG` and `ECHO` wires reversed or missing common ground | Verify pin D11 (Trig) and D12 (Echo) connections with a multimeter | Swap D11 and D12 leads; verify GND bonding between Arduino and L298N |
| Microcontroller resets whenever motors engage | Battery voltage sag caused by motor inrush current | Check 5V rail voltage with oscilloscope or multimeter when motors stall | Add a $470\ \mu\text{F}$ electrolytic capacitor across 5V rail; use dedicated 7.4V pack |
| Servo does not pan or makes grinding noise | Servo horn misaligned or exceeds mechanical 0°-180° limit | Remove horn, set servo angle to 90°, reinstall horn pointing straight ahead | Re-center horn; ensure `config.h` limits angles to $[20^\circ, 160^\circ]$ |
