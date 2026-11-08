# Multi-Sensor Monitoring Node - Pinout & Electrical Specs

**Organization:** Kernelwise Labs  

---

## 1. Pin Interfacing Map

| Sensor / Module | Signal Line | ESP32 GPIO | Bus / Protocol | Electrical Interface |
| :--- | :--- | :--- | :--- | :--- |
| **DHT22** | DATA | GPIO 4 | 1-Wire Digital | 3.3V Logic (4.7kΩ pull-up to 3V3) |
| **BMP280** | SDA | GPIO 21 | I2C Data | 3.3V Logic (Standard I2C bus) |
| | SCL | GPIO 22 | I2C Clock | 3.3V Logic (Standard I2C bus) |
| **LDR Photoresistor** | AOUT | GPIO 36 (ADC1_CH0) | Analog Input | 0.0V – 3.3V voltage divider (10kΩ) |
| **MQ-135 Gas Sensor** | AOUT | GPIO 39 (ADC1_CH3) | Analog Input | 0.0V – 3.3V calibrated signal |
| **Visual Alert LED** | Anode (+) | GPIO 2 | Digital Output | 3.3V Logic via 330Ω resistor |
