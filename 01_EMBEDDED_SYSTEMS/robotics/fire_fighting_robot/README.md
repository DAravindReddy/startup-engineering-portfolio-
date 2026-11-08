# Autonomous Fire Fighting Robot

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Status: Prototype Complete](https://img.shields.io/badge/Status-Prototype%20Complete-green.svg)](PROJECT_STATUS.md)
[![Platform: Arduino UNO](https://img.shields.io/badge/Platform-Arduino%20UNO-blue.svg)](hardware/components_list.md)
[![Tests: 4 Passing](https://img.shields.io/badge/Tests-4%20Passing-brightgreen.svg)](tests/)

An autonomous, 2-wheel differential drive mobile robot engineered for rapid indoor flame detection, directional tracking, and targeted water suppression.

Equipped with a 3-channel optical near-infrared (NIR) flame detector array, an L298N dual H-bridge motor driver, and a high-flow submersible water pump switched via an optoisolated relay, the robot executes a deterministic, non-blocking Finite State Machine (FSM) with automated post-extinguish steam reverification and safety watchdog protections.

---

## Architecture Overview

```mermaid
flowchart LR
    Sensors["3-Ch Flame Array\n(Left / Center / Right)"] --> MCU["ATmega328P Core\n(Fire Fighting FSM)"]
    MCU --> Motors["L298N Motor Driver\n(Differential Drive)"]
    MCU --> Relay["5V Relay Driver\n(Submersible Water Pump)"]
    MCU --> Buzzer["Audible Fire Alarm Strobe"]
```

---

## Directory Structure

```text
fire_fighting_robot/
├── README.md                          # Project overview and quickstart
├── PROJECT_STATUS.md                  # Lifecycle status and verification matrix
├── LICENSE                            # MIT License
├── .gitignore                         # Build and binary ignores
│
├── source_code/                       # Modular C/C++ firmware
│   ├── include/                       # Header interfaces
│   │   ├── config.h                   # Pin definitions, speeds, and timing thresholds
│   │   ├── motor_driver.h             # H-bridge motor driver interface
│   │   ├── flame_sensor_array.h       # 3-channel IR flame array interface
│   │   ├── pump_controller.h          # Water pump relay & watchdog interface
│   │   └── fire_fighting_fsm.h        # Autonomous state machine engine
│   ├── src/                           # Implementation files
│   │   ├── motor_driver.cpp
│   │   ├── flame_sensor_array.cpp
│   │   ├── pump_controller.cpp
│   │   └── fire_fighting_fsm.cpp
│   └── main/                          # Main Arduino sketch
│       └── fire_fighting_robot.ino    # Non-blocking millis() scheduler
│
├── hardware/                          # Electrical and mechanical documentation
│   ├── components_list.md             # Complete BOM and power budget
│   ├── pin_configuration.md           # MCU pinout mapping table
│   ├── hardware_connections.md        # Physical wiring and assembly guide
│   └── circuit_diagram/
│       └── circuit_schematic.md       # Schematic flow diagram
│
├── architecture/                      # Architectural diagrams and models
│   ├── system_architecture.md         # FSM state machine and subsystems
│   └── block_diagram.md               # Hardware block interconnects
│
├── documentation/                     # Technical and operations manuals
│   ├── technical_documentation.md     # NIR flame physics and fluid dynamics
│   ├── functional_requirements.md     # Engineering requirements specification
│   └── installation_guide.md          # Firmware upload and troubleshooting
│
├── whitepaper/                        # IEEE formal research publication
│   └── whitepaper.md                  # Full 13-section research whitepaper
│
└── tests/                             # Unit testing suite
    └── unit_tests/
        └── test_fire_fighting_fsm.cpp # Standalone desktop C++ unit test runner
```

---

## Quickstart

### 1. Compile and Run Desktop Unit Tests
```bash
g++ -Wall -Wextra -Werror -pedantic -std=c++14 -I source_code/include source_code/src/*.cpp tests/unit_tests/test_fire_fighting_fsm.cpp -o test_ff.exe
./test_ff.exe
```

### 2. Upload to Arduino UNO
Open `source_code/main/fire_fighting_robot.ino` in Arduino IDE, select **Arduino Uno**, and click **Upload**.

---

## Maintainer
**Aravind Reddy**  
Founder & Lead Engineer, Kernelwise Labs  
GitHub: [@DAravindReddy](https://github.com/DAravindReddy)
