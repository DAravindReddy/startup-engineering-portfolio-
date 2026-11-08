# Technical Documentation: Smart Home Automation & Irrigation

**Project Name:** `smart_home_automation`  
**Silicon Target:** ATmega328P (Arduino UNO)  
**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  

---

## 1. System Overview

The **Smart Home Automation & Irrigation System** integrates environmental telemetry, home appliance actuation, and closed-loop soil moisture regulation into a non-blocking cooperative multitasking runtime. Controlled locally via Bluetooth (HC-05) and monitored via real-time serialized JSON telemetry, the platform bridges domestic automation with precision agricultural sensing.

---

## 2. Subsystem Implementations

### 2.1 Bluetooth Command Ingestion (`bluetooth_handler.cpp`)
The Bluetooth communication layer interfaces with an HC-05 transceiver running at 9600 baud via Arduino `SoftwareSerial` on Digital Pins 2 (RX) and 3 (TX). 
- Incoming characters are ingested into a 32-byte circular buffer without stalling the main execution thread.
- Line delimiters (`\n` and `\r`) trigger the lexical tokenizer.
- Whitespace is stripped, and commands are matched against predefined command tokens:
  - Appliance commands: `L1_ON`, `L1_OFF`, `L2_ON`, `L2_OFF`
  - Ventilation motor commands: `M_ON`, `M_OFF`, `M_SPEED:<0-255>`
  - Window damper servo commands: `SERVO:<0-180>`
  - Irrigation overrides: `PUMP_ON`, `PUMP_OFF`, `MODE_AUTO`, `MODE_MANUAL`, `SET_TH:<0-100>`
  - Diagnostic queries: `STATUS?`, `GET_SENSORS`

### 2.2 Environmental Telemetry (`sensor_manager.cpp`)
1. **DHT11 Sensor:**
   - Samples ambient room temperature (0–50°C ±2°C) and relative humidity (20–90% RH ±5%).
   - Governed by a 2000ms polling schedule matching the physical capacitive element's response latency.
2. **Soil Moisture Sensor:**
   - Evaluates volumetric soil moisture via a 10-bit analog-to-digital converter (ADC) on pin `A0`.
   - Employs an inverse linear transformation calibrated between dry air ($V_{air} \approx 820$ ADC counts) and water saturation ($V_{wet} \approx 310$ ADC counts):

$$\text{Moisture (\%)} = \left( \frac{\text{ADC}_{\text{air}} - \text{ADC}_{\text{raw}}}{\text{ADC}_{\text{air}} - \text{ADC}_{\text{wet}}} \right) \times 100$$

### 2.3 Closed-Loop Irrigation FSM (`irrigation_system.cpp`)
To eliminate relay chatter and actuator wear, the irrigation system enforces a finite state machine with dual-threshold hysteresis:
- **`PUMP_STATE_IDLE`:** Pump is de-energized. Transitions to `PUMP_STATE_WATERING` only when calibrated soil moisture drops below the configurable dry threshold (Default: 35%).
- **`PUMP_STATE_WATERING`:** Pump relay is energized. Water is delivered to plant root zone. Remains active until moisture surpasses the wet threshold (Default: 65%), or the **Safety Watchdog** trips.
- **`PUMP_STATE_COOLDOWN`:** Minimum rest period (5000ms) enforced after each watering cycle to allow water percolation before re-evaluating moisture.
- **`PUMP_STATE_SAFETY_TRIP`:** Emergency trip if continuous pump operation exceeds 15,000ms. Requires explicit manual reset to prevent flooding due to broken supply lines or sensor displacement.

### 2.4 Power Actuation (`actuator_controller.cpp`)
- **DC Ventilation Motor:** Driven through an L298N Dual Full-Bridge Driver. Pulse-Width Modulation (PWM) on pin 6 (Timer0, 980 Hz) provides linear speed control from 0 to 255.
- **Servo Motor:** Driven via Timer1 (pin 9) with 50 Hz PWM, mapping 0° to 180° for mechanical damper ventilation.
- **Relay Channel:** Driven via pin 10 with active-LOW optocoupled galvanic isolation, switching the 5V/12V submersible DC pump.
