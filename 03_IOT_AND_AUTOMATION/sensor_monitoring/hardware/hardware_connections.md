# Multi-Sensor Monitoring Node - Hardware Interconnects

**Organization:** Kernelwise Labs  

---

## 1. Electrical Wiring Topology

```
[5V 2A USB Supply] -----> [ESP32 VIN] & [MQ-135 Heater VCC]
                              |
                     [ESP32 Onboard 3.3V LDO]
                              |
               +--------------+--------------+
               |                             |
         [DHT22 VCC]                    [BMP280 VCC]
         [LDR Divider Top]              [Pull-Up Resistors]

[Sensor Connections to ESP32]
  - DHT22 DATA     -----> GPIO 4 (with 4.7kΩ pull-up to 3.3V)
  - BMP280 SDA     -----> GPIO 21
  - BMP280 SCL     -----> GPIO 22
  - LDR Midpoint   -----> GPIO 36 (ADC1_CH0)
  - MQ-135 AOUT    -----> GPIO 39 (ADC1_CH3)
  - Alert LED      -----> GPIO 2  (with 330Ω in series to GND)
```

---

## 2. Decoupling & Ground Integrity

- All analog ground references (LDR bottom resistor, MQ-135 ground) connect to a star ground junction near the ESP32 `GND` pin to minimize ground bounce.
- A 100nF ceramic bypass capacitor is connected directly across the BMP280 $V_{CC}$ and $GND$ pins to decouple digital bus switching noise.
