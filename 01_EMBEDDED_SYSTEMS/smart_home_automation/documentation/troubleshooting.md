# Hardware and Software Troubleshooting Guide

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Diagnostic Fault Tree

```mermaid
flowchart TD
    Issue["System Malfunction Detected"] --> Q1{"Is Status Heartbeat<br/>LED (Pin 13) Blinking?"}
    
    Q1 -- No --> F1["Power Rail Failure or MCU Freeze.<br/>Check 5V rail with multimeter.<br/>Check USB cable / Vin connection."]
    Q1 -- Yes --> Q2{"Does HC-05 respond to<br/>Bluetooth commands?"}

    Q2 -- No --> F2["Inspect Pin 2 (RX) and Pin 3 (TX).<br/>Verify 1k/2k voltage divider.<br/>Ensure baud rate is set to 9600."]
    Q2 -- Yes --> Q3{"Is Soil Moisture reading<br/>stuck at 0% or 100%?"}

    Q3 -- Yes --> F3["Inspect A0 analog wiring.<br/>Recalibrate SOIL_RAW_AIR_DRY and<br/>SOIL_RAW_WATER_WET constants in config.h."]
    Q3 -- No --> Q4{"Does Pump Fail to<br/>Activate in Auto Mode?"}

    Q4 -- Yes --> F4["Check if Irrigation Mode is set to MANUAL.<br/>Verify if Safety Watchdog tripped.<br/>Send 'PUMP_OFF' then 'MODE_AUTO' to reset."]
```

---

## 2. Common Symptoms and Root Cause Analysis

| Symptom | Probable Root Cause | Corrective Action |
|---|---|---|
| **HC-05 paired but won't send/receive text** | Inverted TX/RX lines or incorrect baud rate. | Swap Pin 2 and Pin 3 connections; ensure terminal app uses 9600 baud and appends `\r\n`. |
| **Arduino resets when DC Motor starts** | High inrush current drawing down 5V rail. | Power L298N motor driver from external 12V supply; do not power motor from Arduino 5V header. |
| **Water pump turns on briefly and stays off** | Safety Watchdog tripped due to dry sensor or prolonged run time. | Check water reservoir level and probe placement. Send `PUMP_OFF` to clear error state. |
| **DHT11 returns invalid readings** | Missing pull-up resistor or read frequency > 0.5 Hz. | Install a 10kΩ pull-up resistor between 5V and DATA. Ensure read interval $\ge 2000$ms. |
