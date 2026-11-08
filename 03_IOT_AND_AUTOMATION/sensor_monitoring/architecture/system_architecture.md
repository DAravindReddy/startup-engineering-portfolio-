# Multi-Sensor Monitoring Node - System Architecture & Diagrams

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  

---

## 1. Hardware Block Diagram

```mermaid
flowchart TD
    subgraph Power ["Power Subsystem"]
        USB["5V 2A USB Supply"] --> ESP_LDO["ESP32 3.3V LDO"]
        USB --> MQ_Heater["MQ-135 5V Heater Rail"]
    end

    subgraph SensorArray ["Environmental Sensor Bus"]
        DHT["DHT22 (Temp & Humidity)"] -->|1-Wire Digital| GPIO4["GPIO 4"]
        BMP["BMP280 (Pressure)"] -->|I2C SDA/SCL| I2C_Pins["GPIO 21 / 22"]
        LDR["LDR Photoresistor (Light)"] -->|Analog 0-3.3V| GPIO36["GPIO 36 (ADC1_CH0)"]
        MQ["MQ-135 (Air Quality)"] -->|Analog 0-3.3V| GPIO39["GPIO 39 (ADC1_CH3)"]
    end

    subgraph EdgeEngine ["ESP32 Firmware Engine"]
        RingBuf["Rolling Ring Buffer (32 frames)"]
        StatCalc["Statistical Engine (Mean, Min, Max, StdDev)"]
        AlertMgr["Multi-Tier Alert Evaluator"]
        JSONFormat["JSON Telemetry Serializer"]

        GPIO4 & I2C_Pins & GPIO36 & GPIO39 --> RingBuf
        RingBuf --> StatCalc
        RingBuf --> AlertMgr
        AlertMgr --> AlertLED["GPIO 2 Status LED"]
        RingBuf & StatCalc & AlertMgr --> JSONFormat
    end

    subgraph Communication ["Telemetry Uplink"]
        JSONFormat --> SerialStream["Serial Console (115200 baud)"]
        JSONFormat --> MQTTStream["WiFi MQTT Client (kw/sensors/telemetry)"]
    end
```

---

## 2. Telemetry Processing Pipeline

1. **Acquisition:** Sensor bus is sampled every $500\text{ ms}$ under cooperative non-blocking scheduling.
2. **Buffering:** Ingests readings into an in-memory ring buffer retaining the last 32 frames ($16\text{ seconds}$ historical window).
3. **Statistical Aggregation:** Computes mean, variance, and sample standard deviation per channel to identify sudden environmental surges.
4. **Alert Classification:** Detects out-of-spec conditions:
   - Warning Temperature ($\ge 38^\circ\text{C}$), Critical Temperature ($\ge 50^\circ\text{C}$).
   - Warning Air Quality ($\ge 200\text{ ppm}$), Critical Air Quality ($\ge 400\text{ ppm}$).
   - Atmospheric Pressure Sudden Drop ($\le 950\text{ hPa}$, storm indicator).
5. **Serialization & Streaming:** Generates RFC 8259-compliant JSON payloads published to MQTT brokers.
