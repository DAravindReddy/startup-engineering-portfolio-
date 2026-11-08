# Arduino Nano 33 BLE Sense AI - Bill of Materials (BOM) & Hardware Specifications

## 1. Components Summary

| Component Item | Description / Model | Quantity | Operating Voltage | Current Consumption | Role in System |
|:---|:---|:---:|:---:|:---:|:---|
| **Edge AI MCU Board** | Arduino Nano 33 BLE Sense (Nordic nRF52840) | 1 | 3.3V Logic (5V Vin via USB) | ~15 mA (Peak with BLE ~25 mA) | 64 MHz Cortex-M4F, 1MB Flash, 256KB RAM |
| **9-Axis Motion IMU** | ST LSM9DS1 (or Bosch BMI270/BMM150 on Rev2)| Onboard | 3.3V Internal | 4.6 mA | 100 Hz 3-axis accelerometer vibration sensing |
| **Environmental Sensor** | ST HTS221 Temperature & Humidity Sensor | Onboard | 3.3V Internal | 2.0 μA | Operating ambient and machine thermal monitoring|
| **Digital Microphone** | ST MP34DT05 Omnidirectional MEMS Mic | Onboard | 3.3V Internal | 650 μA | Acoustic machine harmonic audio sampling |
| **BLE 5.0 Radio** | Nordic nRF52840 Integrated 2.4 GHz Transceiver| Onboard | 3.3V Internal | 4.8 mA (Tx @ 0 dBm) | GATT Health Service telemetry broadcast |
| **Power Source** | Micro-USB Cable or 3.7V LiPo with 3.3V LDO | 1 | 5.0V / 3.7V | 50 mA continuous | Machine or battery power |

---

## 2. Ultra-Low Power Budget Analysis

```
Total Active System Power Profile (nRF52840 @ 64 MHz):
├── CPU Core Running TinyML Inference (2 Hz): ~7.5 mA
├── LSM9DS1 100 Hz Accelerometer Active:      ~3.2 mA
├── HTS221 Environmental Sensor:             ~0.01 mA
└── BLE 5.0 Advertising / Notifications:      ~4.8 mA (average pulse)

Mean Operating Current:  ~15.5 Milliamperes at 3.3V (~51 Milliwatts)
Battery Life (500mAh LiPo Cell): ~32 Hours continuous autonomous streaming
Sleep Mode Optimized (BLE Advertising Interval 1s): > 6 Months on coin cell (CR2032)
```
