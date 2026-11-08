# Multi-Sensor Monitoring Node - Bill of Materials (BOM)

**Project:** Multi-Sensor Environmental Telemetry Node  
**Organization:** Kernelwise Labs  
**Target Hardware:** ESP32-WROOM-32 / NodeMCU  

---

| Item | Component | Specification | Quantity | Primary Purpose |
| :--- | :--- | :--- | :--- | :--- |
| 1 | Microcontroller | ESP32-WROOM-32 (240 MHz dual-core, 520 KB SRAM, 2.4 GHz WiFi/BLE) | 1 | Sensor sampling, statistical aggregation, MQTT publishing |
| 2 | Climate Sensor | DHT22 / AM2302 (Digital relative humidity & temperature) | 1 | Ambient air climate tracking |
| 3 | Barometer / Altimeter | BMP280 (I2C Digital Barometric Pressure Sensor, 300–1100 hPa) | 1 | Atmospheric pressure and altitude estimation |
| 4 | Ambient Light Sensor | GL5528 Photoresistor (LDR) in voltage divider configuration | 1 | Ambient illuminance monitoring (Lux) |
| 5 | Air Quality / Gas Sensor | MQ-135 Gas Sensor (Sensitive to NH3, NOx, Alcohol, Benzene, Smoke, CO2) | 1 | Hazardous gas & IAQ index measurement |
| 6 | Alert Indicator | 5mm High-Intensity Red LED with 330Ω ballast resistor | 1 | Visual warning / critical alert indicator |
| 7 | Power Supply | 5V 2A Micro-USB / USB-C DC Regulated Power Adapter | 1 | Primary power source |
