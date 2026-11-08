# System Architecture Specification

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  
**Domain:** Embedded Automation & Mechatronic Control  
**Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  

---

## 1. Architectural Overview

The Smart Home Automation and Closed-Loop Irrigation system is structured as a hierarchical embedded architecture decoupled into distinct physical, driver, logic, and presentation layers:

```mermaid
flowchart TD
    subgraph Layer_User ["User Interface & Telemetry Layer"]
        MobileApp["Android / PC Bluetooth Terminal"]
        SerialMonitor["USB Debug Serial Console"]
    end

    subgraph Layer_Comm ["Communication & Protocol Layer"]
        BT_Driver["SoftwareSerial Driver (Pins 2/3)"]
        Parser["Non-blocking Command Tokenizer"]
        TelemetryFmt["JSON & Status String Serializer"]
    end

    subgraph Layer_Control ["Core Control & State Machine Layer"]
        SuperLoop["Cooperative Multitasking Scheduler (millis)"]
        IrrigationFSM["Closed-Loop Irrigation FSM with Hysteresis"]
        SafetyWatchdog["Pump Run-Time Safety Supervisor (15s Max)"]
        ActuatorState["Actuator State Registry"]
    end

    subgraph Layer_HAL ["Hardware Abstraction & Sensor Management"]
        DHT_HAL["DHT11 1-Wire Reader"]
        Soil_HAL["Soil Moisture ADC Normalizer"]
        PWM_HAL["Timer0 / Timer1 PWM Controllers"]
        GPIO_HAL["Digital Pin Interface"]
    end

    subgraph Layer_Physical ["Physical Silicon & Actuators Layer"]
        Sensors["DHT11 + Resistive Soil Probe"]
        Actuators["L298N DC Motor + SG90 Servo + Relay Pump + LEDs"]
    end

    MobileApp <--> BT_Driver
    SerialMonitor <--> SuperLoop

    BT_Driver --> Parser
    Parser --> ActuatorState
    Parser --> IrrigationFSM
    ActuatorState --> TelemetryFmt
    TelemetryFmt --> BT_Driver

    SuperLoop --> IrrigationFSM
    SuperLoop --> DHT_HAL
    SuperLoop --> Soil_HAL

    Soil_HAL --> IrrigationFSM
    IrrigationFSM --> SafetyWatchdog
    SafetyWatchdog --> ActuatorState

    ActuatorState --> GPIO_HAL
    ActuatorState --> PWM_HAL

    DHT_HAL <--> Sensors
    Soil_HAL <--> Sensors
    GPIO_HAL --> Actuators
    PWM_HAL --> Actuators
```

---

## 2. Key Architectural Design Decisions

1. **Zero-Blocking Execution Loop:**
   To guarantee rapid command responsiveness (<50ms latency) over Bluetooth, traditional `delay()` statements are eliminated. The system uses timestamp differential scheduling (`millis() - last_time >= interval`), maintaining high loop throughput.
2. **Dual-Mode Irrigation with Safety Interlock:**
   The irrigation sub-system operates under two mutually exclusive modes:
   - `AUTO Mode`: Autonomous closed-loop control governed by soil moisture hysteresis.
   - `MANUAL Mode`: Remote operator override.
   In both modes, a hardware safety watchdog trips the pump if continuous actuation exceeds 15 seconds, preventing catastrophic flooding in the event of dry-well conditions or probe dislocation.
3. **Hardware Independence:**
   Core control algorithms (command parsing, moisture percentage mapping, and hysteresis state logic) are completely separated from Arduino hardware headers, allowing native C++ unit testing on development workstations.
