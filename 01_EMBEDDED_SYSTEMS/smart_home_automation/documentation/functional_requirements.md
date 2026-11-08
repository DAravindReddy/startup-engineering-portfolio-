# Functional Requirements Traceability Matrix

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

| Req ID | Requirement Description | Implementation Module | Verification Method | Status |
|:---:|---|---|---|:---:|
| **FR-01** | Support wireless Bluetooth control using standard ASCII commands. | `bluetooth_handler.cpp` | Unit test & live serial terminal test. | **VERIFIED** |
| **FR-02** | Independent remote toggling of domestic electrical appliances / LEDs. | `actuator_controller.cpp` | GPIO state assertion & oscilloscope. | **VERIFIED** |
| **FR-03** | Continuous non-blocking monitoring of ambient temperature & humidity. | `sensor_manager.cpp` | Timing scheduler & console telemetry stream. | **VERIFIED** |
| **FR-04** | Variable speed control of DC ventilation motor via PWM. | `actuator_controller.cpp` | Timer0 PWM frequency & duty cycle check. | **VERIFIED** |
| **FR-05** | Precise angular positioning of mechanical servo damper (0° - 180°). | `main.ino` (Servo.h) | Servo angle calibration test. | **VERIFIED** |
| **FR-06** | Real-time volumetric soil moisture sampling with linear calibration. | `sensor_manager.cpp` | ADC simulation & dry/wet boundary tests. | **VERIFIED** |
| **FR-07** | Automatic activation of submersible water pump when moisture < threshold. | `irrigation_system.cpp` | Closed-loop state machine unit tests. | **VERIFIED** |
| **FR-08** | Dual irrigation modes: Autonomous closed-loop and manual operator override. | `irrigation_system.cpp` | Mode switching unit tests. | **VERIFIED** |
| **FR-09** | Anti-cycling hysteresis and safety maximum run-time cutoff (15s). | `irrigation_system.cpp` | Watchdog timeout unit tests. | **VERIFIED** |
| **FR-10** | Periodic and on-demand JSON telemetry broadcasting device and sensor status. | `main.ino` | Serial monitor JSON schema validation. | **VERIFIED** |
