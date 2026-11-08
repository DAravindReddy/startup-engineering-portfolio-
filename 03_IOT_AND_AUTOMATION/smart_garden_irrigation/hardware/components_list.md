# Smart Garden Irrigation - Bill of Materials (BOM)

**Project:** Smart Garden Irrigation & Soil Monitoring System  
**Organization:** Kernelwise Labs  
**Target Hardware:** ESP32-WROOM-32 / Arduino UNO / Mega  

---

| Item | Component | Specification | Quantity | Primary Purpose |
| :--- | :--- | :--- | :--- | :--- |
| 1 | Microcontroller | ESP32-WROOM-32 (Dual-core Xtensa 240 MHz, 520 KB SRAM, WiFi/BLE) | 1 | Edge compute, FSM execution, MQTT streaming |
| 2 | Soil Moisture Sensor | Capacitive Soil Moisture Sensor v1.2 (Corrosion resistant, 0–3.3V analog) | 1 | Volumetric soil water content measurement |
| 3 | Environmental Sensor | DHT22 / AM2302 (Calibrated digital temperature & humidity) | 1 | Ambient evapotranspiration estimation |
| 4 | Power Relay Module | 1-Channel 5V Relay with Optocoupler Isolation (10A 250VAC / 10A 30VDC) | 1 | Galvanically isolated water pump switching |
| 5 | Water Pump | 12V Submersible DC Centrifugal Water Pump (240 L/h, 3m head) | 1 | Pressurized irrigation delivery |
| 6 | Flyback Protection Diode | 1N4007 (1000V 1A Fast Recovery Silicon Diode) | 1 | Inductive kickback suppression across DC pump |
| 7 | Power Supply | 12V 2A DC Regulated Switch-Mode Power Supply (SMPS) | 1 | Primary input power |
| 8 | Step-Down Buck Converter | LM2596 (12V to 5V 3A DC-DC Step Down) | 1 | Regulated MCU logic bus supply |
| 9 | Status Indicators | 3mm Green / Red LEDs with 330 Ohm current-limiting resistors | 2 | Visual telemetry and system heartbeat |
