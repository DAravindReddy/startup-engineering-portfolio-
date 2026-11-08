# Smart Garden Irrigation - Architecture Diagrams

**Organization:** Kernelwise Labs  

---

## 1. Hardware Block Diagram

```mermaid
flowchart LR
    subgraph PowerSystem ["Power Distribution Network"]
        SMPS["12V 2A SMPS"] --> LM2596["LM2596 (5V 3A Buck)"]
        SMPS --> RelayCOM["Relay Common"]
        LM2596 --> ESP32_VIN["ESP32 VIN"]
        LM2596 --> RelayVCC["Relay VCC"]
    end

    subgraph SensingUnit ["Sensor Interface"]
        Soil["Capacitive Soil Sensor"] -->|Analog 0-3.3V| GPIO34["GPIO 34 (ADC1_CH6)"]
        DHT["DHT22 Temp & Humidity"] -->|1-Wire Digital| GPIO4["GPIO 4"]
    end

    subgraph ActuationUnit ["Power Actuation"]
        GPIO23["GPIO 23 (Relay Trigger)"] --> OptoRelay["Optocoupled Relay"]
        OptoRelay --> Pump["12V Submersible DC Pump"]
        Pump --- Diode["1N4007 Flyback Diode"]
    end
```

---

## 2. Software Data Flow Diagram

```mermaid
flowchart TD
    RawADC["Raw ADC Soil Sample"] --> EMAFilter["EMA Noise Filter (alpha=0.25)"]
    DHTRaw["DHT22 Raw Frame"] --> TempComp["Evapotranspiration Boost Engine"]

    EMAFilter --> MoistureValue["Filtered Moisture %"]
    TempComp --> DynThresholds["Dynamic Low/High Thresholds"]

    MoistureValue --> StateEvaluator{"FSM State Evaluator"}
    DynThresholds --> StateEvaluator

    StateEvaluator -->|Moisture < Low| ActivatePump["Activate Pump Relay & Start Timer"]
    StateEvaluator -->|Moisture >= High or Timeout| DeactivatePump["Deactivate Pump Relay & Start Cooldown"]
    StateEvaluator -->|Cooldown Expired| ReadyIdle["Transition to IDLE"]

    MoistureValue --> MQTTStream["JSON Telemetry Formatter -> MQTT Client"]
    StateEvaluator --> MQTTStream
```

---

## 3. Component Architecture

```
+-------------------------------------------------------------------------------+
|                            IRRIGATION CONTROLLER                              |
+-------------------------------------------------------------------------------+
| 1. Sensor Processing: EMA Filter, ADC Linearizer, Out-of-bounds Detector     |
| 2. Climate Model: Evapotranspiration compensation, threshold adaptation       |
| 3. Safety Watchdog: Max 15s burst limit, 10s soil infiltration cooldown        |
| 4. Actuator Driver: Active-low relay state machine                            |
| 5. Remote Gateway: MQTT JSON telemetry streaming & command dispatcher         |
+-------------------------------------------------------------------------------+
```
