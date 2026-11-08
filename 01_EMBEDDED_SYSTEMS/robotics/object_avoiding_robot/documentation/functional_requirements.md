# Object Avoiding Robot - Functional Requirements & Software Design

## 1. Functional Requirements

- **FR-01 (Continuous Sonar Acquisition)**: The system shall trigger HC-SR04 ultrasonic range measurements at a rate of 20 Hz (every 50 ms) during forward motion.
- **FR-02 (Threshold Alerting & Halting)**: When forward obstacle clearance drops to or below 30 cm (`DISTANCE_WARNING_CM`), the robot shall halt both motors within 20 ms.
- **FR-03 (Automated Turret Scanning)**: Following a warning halt, the robot shall pan the ultrasonic sensor 70° to the left (160° heading) and 70° to the right (20° heading), allowing 250 ms mechanical settling time at each angle before recording spatial clearance.
- **FR-04 (Optimal Path Selection)**: The navigation controller shall compare left vs. right clearances and pivot the robot toward the path with greater clearance.
- **FR-05 (Dead-End Escape Maneuver)**: If clearances in both left and right directions are less than 30 cm, the robot shall reverse for 300 ms before executing a 180° turnaround pivot.
- **FR-06 (Emergency Cutoff)**: If forward distance drops below 15 cm (`DISTANCE_CRITICAL_CM`), the robot shall immediately reverse without scanning.

---

## 2. Software Design & Task Scheduling

The software is structured as non-blocking cooperative state machines:
- **`nav_fsm_update()`**: Executed every 20 ms (50 Hz). Evaluates sensor transitions and timers.
- **Heartbeat & Telemetry Task**: Executed every 1000 ms (1 Hz). Toggles status LED and outputs serial telemetry at 115200 baud.
- **No blocking `delay()` in `loop()`**: All delays (e.g. servo settling, pivot turning) use non-blocking millisecond difference checks (`now - entry_time >= timeout`).
