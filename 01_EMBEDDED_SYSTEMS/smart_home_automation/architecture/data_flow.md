# Data Flow & Sequence Architecture

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Closed-Loop Irrigation Data Flow

```mermaid
sequenceDiagram
    autonumber
    participant Sensor as Soil Moisture Sensor (A0)
    participant ADC as ATmega328P ADC Engine
    participant Logic as Irrigation Controller (FSM)
    participant Timer as Millis Watchdog
    participant Relay as Optocoupled Relay
    participant Pump as Submersible Water Pump

    Note over Sensor,ADC: Periodic Polling (1000ms Interval)
    Sensor->>ADC: Analog Voltage (0V - 5V)
    ADC->>Logic: 10-bit Raw Reading (0 - 1023)
    Logic->>Logic: Calibrate to Moisture % (0 - 100%)

    alt Moisture < Dry Threshold (e.g., 35%)
        Logic->>Timer: Record pump_start_time_ms = millis()
        Logic->>Relay: Set PIN_WATER_PUMP_RELAY LOW (Active)
        Relay->>Pump: Energize 5V Pump Coil
        Note over Relay,Pump: Pump Actively Irrigating
    else Moisture >= Wet Threshold (e.g., 65%)
        Logic->>Relay: Set PIN_WATER_PUMP_RELAY HIGH (De-energize)
        Relay->>Pump: Cut DC Power
        Logic->>Timer: Record pump_stop_time_ms = millis() (Begin Cooldown)
    else Pump Active for > 15,000ms (Watchdog Trip)
        Timer-->>Logic: Timeout Exceeded!
        Logic->>Relay: Emergency Cutoff (Force HIGH)
        Relay->>Pump: Immediate Shutoff
        Logic->>Logic: Transition to PUMP_STATE_SAFETY_TRIP
    end
```

---

## 2. Bluetooth Command & Telemetry Sequence

```mermaid
sequenceDiagram
    autonumber
    actor User as Mobile App / Operator
    participant BT as HC-05 Module
    participant Ring as RX Ingestion Buffer
    participant Parser as Command Parser
    participant Act as Actuator Dispatcher
    participant Telemetry as Telemetry Serializer

    User->>BT: Wireless ASCII string ("L1_ON\n" or "SET_TH:40\n")
    BT->>Ring: Serial Byte Stream (9600 Baud)
    Ring->>Parser: Delimited Token String
    Parser->>Act: Validated Command (Type, Param)
    Act->>Act: Update Digital Pin / PWM State
    Act->>BT: "[ACK] L1 ON" Confirmation
    BT-->>User: Wireless Confirmation Display

    Note over Telemetry,User: Periodic Broadcast (Every 3000ms)
    Telemetry->>BT: JSON String (temp, hum, soil_pct, pump, mode)
    BT-->>User: Update Live Telemetry Cards on Dashboard
```
