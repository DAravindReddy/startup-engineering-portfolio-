# Fire Fighting Robot - Installation & Troubleshooting Guide

## 1. Toolchain & Firmware Setup

### Desktop Unit Testing
Execute the standalone C++14 unit test harness with GCC:
```bash
cd 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot
g++ -Wall -Wextra -Werror -pedantic -std=c++14 -I source_code/include source_code/src/*.cpp tests/unit_tests/test_fire_fighting_fsm.cpp -o test_ff.exe
./test_ff.exe
```

### Flashing Arduino Firmware
1. Open `source_code/main/fire_fighting_robot.ino` in Arduino IDE.
2. Select Board: **Arduino Uno**.
3. Select Port: **COMx**.
4. Click **Upload** (`Ctrl+U`).

---

## 2. Troubleshooting Matrix

| Problem / Observation | Probable Root Cause | Verification Step | Remediation |
|:---|:---|:---|:---|
| Flame sensors trigger without any flame present | Potentiometer sensitivity is set too high, reacting to incandescent indoor lighting | Check onboard sensor SMD LEDs under ambient light | Turn sensor multi-turn trimmer clockwise until LED turns off under ambient light |
| Robot turns away from flame instead of toward it | Left and right flame sensor wires swapped, or motor polarity inverted | Trigger left sensor with lighter flame and observe wheel direction | Swap D2 and D4 inputs, or invert motor leads at L298N terminals |
| Water pump does not spray when relay clicks | Air lock inside mini centrifugal pump, or dry run without priming | Listen for pump motor hum inside reservoir | Prime tubing by siphoning or filling reservoir fully; verify pump supply rail |
| Arduino reboots immediately when pump activates | Inductive back-EMF or inrush current causing logic voltage drop | Monitor 5V pin on oscilloscope during relay trigger | Ensure relay module is optocoupled; add a flyback diode (1N4007) across pump leads |
