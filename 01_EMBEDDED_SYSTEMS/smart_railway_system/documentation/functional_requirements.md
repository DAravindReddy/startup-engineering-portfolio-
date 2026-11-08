# Smart Railway Track System - Functional Requirements & Software Design

## 1. Functional Requirements

- **FR-01 (Dual-Rail Fracture Sampling)**: The system shall poll both the left and right rail optical infrared reflectance sensors every 50 ms.
- **FR-02 (Crack Temporal Debouncing)**: Discontinuities on either rail must persist continuously for $\ge 40\text{ ms}$ before transitioning the system to `RAIL_FSM_STATE_CRACK_DANGER`.
- **FR-03 (Obstacle Clearance Measurement)**: The system shall trigger ultrasonic sonar pings to monitor foreign track obstructions up to 400 cm.
- **FR-04 (Multi-Tier Signal Interlocking)**:
  - If a crack is confirmed on either rail $\rightarrow$ Set Red Aspect, lower barrier to 0°, activate buzzer, stop trolley.
  - Else if obstacle distance $\le 30\text{ cm}$ $\rightarrow$ Set Red Aspect, lower barrier to 0°, activate buzzer, stop trolley.
  - Else if obstacle distance $\le 60\text{ cm}$ $\rightarrow$ Set Yellow Caution Aspect, reduce trolley speed by 50%.
  - Else $\rightarrow$ Set Green Aspect, elevate barrier to 90°, maintain cruise speed.
- **FR-05 (Serial Telemetry Broadcast)**: The system shall broadcast real-time operational state, aspect name, obstacle distance, and crack statuses at 1 Hz over UART at 115200 baud.

---

## 2. Software Design & Implementation Architecture

```text
source_code/
├── include/
│   ├── config.h                   # Thresholds, debounce timings, pin assignments
│   ├── crack_detector.h           # Dual IR rail sensor interface & debouncer
│   ├── obstacle_sonar.h           # Ultrasonic rangefinder interface
│   ├── signal_gate_controller.h   # Multi-aspect signal and servo barrier controller
│   └── railway_fsm.h              # Safety priority finite state machine
└── src/
    ├── crack_detector.cpp
    ├── obstacle_sonar.cpp
    ├── signal_gate_controller.cpp
    └── railway_fsm.cpp
```
