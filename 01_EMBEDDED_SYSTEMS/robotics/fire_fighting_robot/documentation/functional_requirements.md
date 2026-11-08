# Fire Fighting Robot - Functional Requirements & Software Design

## 1. Functional Requirements

- **FR-01 (Multi-Angle Flame Sensing)**: The system shall poll 3 distinct infrared flame sensor channels (Left at $-45^\circ$, Center at $0^\circ$, Right at $+45^\circ$) every 20 ms.
- **FR-02 (Directional Alignment)**: When a flame is detected on the left or right channels, the robot shall execute differential pivot steering toward the detected source until the center channel aligns with the flame.
- **FR-03 (Controlled Fire Approach)**: Once aligned along the forward centerline, the robot shall advance toward the fire at a reduced approach speed (`SPEED_APPROACH = 140 PWM`).
- **FR-04 (Pulsed Water Extinguishing)**: Upon reaching extinguishing range, the robot shall stop all drive motors and activate the water pump relay for exactly 3.0 seconds.
- **FR-05 (Extinguish Verification)**: Following water discharge, the system shall wait 1.5 seconds for water vapor and steam turbulence to clear, then re-sample the flame detectors:
  - If the fire is extinguished, the robot shall reset counters and resume patrol mode.
  - If the fire remains active, the robot shall initiate another spray cycle (up to a maximum of 3 consecutive bursts).
- **FR-06 (Flooding and Burnout Safety Lockout)**: If the flame persists after 3 consecutive spray cycles, the system shall enter `FF_STATE_FAULT_HALT`, disengaging the pump and activating the alarm buzzer continuously to prevent pump burnout and localized water flooding.

---

## 2. Software Design & Implementation Architecture

```text
source_code/
├── include/
│   ├── config.h               # Pin definitions, speeds, and timing thresholds
│   ├── motor_driver.h         # Differential motor primitives (Forward, Reverse, Pivot)
│   ├── flame_sensor_array.h   # 3-channel IR comparator sampling & position classification
│   ├── pump_controller.h      # Water relay activation and watchdog timeout protection
│   └── fire_fighting_fsm.h    # Autonomous state machine engine
└── src/
    ├── motor_driver.cpp
    ├── flame_sensor_array.cpp
    ├── pump_controller.cpp
    └── fire_fighting_fsm.cpp
```
