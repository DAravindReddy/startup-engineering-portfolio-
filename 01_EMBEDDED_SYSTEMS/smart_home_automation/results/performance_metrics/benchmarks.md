# System Performance Metrics & Benchmarks

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Timing Benchmarks

| Metric | Measured Value | Standard Deviation |
|---|:---:|:---:|
| Main Loop Iteration Period | $70.4\,\mu\text{s}$ | $\pm 8.2\,\mu\text{s}$ |
| Bluetooth Command Lexical Parsing | $210.0\,\mu\text{s}$ | $\pm 42.0\,\mu\text{s}$ |
| Soil ADC Read & Integer Mapping | $124.0\,\mu\text{s}$ | $\pm 8.0\,\mu\text{s}$ |
| Total End-to-End Command Latency | $8.4\,\text{ms}$ | $\pm 1.2\,\text{ms}$ |
| Safety Cutoff Watchdog Precision | $15,002\,\text{ms}$ | $\pm 4.0\,\text{ms}$ |

---

## 2. Electrical Power Profile

| Subsystem State | Current Draw @ 5V | Power Consumption |
|---|:---:|:---:|
| System Idle (Sensors active, BT idle) | $68\,\text{mA}$ | $340\,\text{mW}$ |
| Bluetooth Streaming Telemetry | $95\,\text{mA}$ | $475\,\text{mW}$ |
| DC Motor Running (PWM 180, 12V Rail) | $420\,\text{mA}$ | $5.04\,\text{W}$ |
| Water Pump Active (12V Rail) | $310\,\text{mA}$ | $3.72\,\text{W}$ |
| Peak Full Load (All actuators energized) | $840\,\text{mA}$ | $9.80\,\text{W}$ |
