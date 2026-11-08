# Arduino Nano 33 BLE Sense AI - Pin Configuration & Sensor Mapping

## 1. Internal Sensor Bus Interconnects (Nordic nRF52840)

| Sensor Module | Physical Interface | Internal Bus / GPIO | Address / Line | Function |
|:---|:---|:---|:---|:---|
| **LSM9DS1 IMU** | Two-Wire Interface (I2C) | Internal I2C Bus | `0x6B` (Accel/Gyro), `0x1E` (Mag) | 100 Hz 3-Axis Vibration Acceleration |
| **HTS221 Temp/Humidity** | Two-Wire Interface (I2C) | Internal I2C Bus | `0x5F` | Thermal Zone & Humidity Monitoring |
| **APDS9960 Optical** | Two-Wire Interface (I2C) | Internal I2C Bus | `0x39` | Ambient Optical Proximity & Color |
| **MP34DT05 Mic** | PDM Interface | PDM Clock (P0.26), PDM Data (P0.25)| Digital Pulse-Density Modulation | Acoustic Noise Floor & Bearing Squeal |
| **BLE Radio** | Internal SoC 2.4 GHz | Ant. Balun Network | 2.4 GHz ISM Band | BLE GATT Characteristic Advertising |
| **Status LED** | GPIO Output | Pin 13 / `LED_BUILTIN` | P0.13 | Heartbeat & Anomaly Alert Strobe |

---

## 2. External I/O Expansion Header

| Nano Pin | Function | Voltage Tolerance | Recommended External Role |
|:---:|:---|:---:|:---|
| **VUSB / VIN** | Power Input | 4.5V - 21.0V DC | External DC adapter or battery input |
| **+3V3** | Logic Output Rail | 3.3V DC (Max 800 mA) | Power supply for external industrial relays/sensors |
| **GND** | System Ground | 0V | Ground reference |
| **D2** | External Fault Trigger | 3.3V TTL | Industrial PLC emergency interlock input |
| **D3** | Alarm Relay Output | 3.3V TTL | External industrial strobe / contactor disconnect |
