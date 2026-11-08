# Arduino Nano 33 BLE Sense AI - Circuit Schematic & Architecture

## 1. Onboard SoC Architecture & Sensor Topology

```
+-----------------------------------------------------------------------------------+
|                        Arduino Nano 33 BLE Sense Board                            |
|                                                                                   |
|  +-----------------------------------------------------------------------------+  |
|  |                 Nordic Semiconductor nRF52840 SoC                           |  |
|  |   - ARM Cortex-M4F with FPU @ 64 MHz                                        |  |
|  |   - 1 MB Flash, 256 KB RAM                                                  |  |
|  |   - 2.4 GHz Multi-Protocol Transceiver (Bluetooth 5.0 Low Energy)           |  |
|  +-------------------------------------+---------------------------------------+  |
|                                        |                                          |
|                 Internal I2C Bus       |             PDM Audio Bus                |
|              +-------------------------+--------+------------------+              |
|              |                         |                           |              |
|              v                         v                           v              |
|     +------------------+      +------------------+        +------------------+    |
|     | ST LSM9DS1 IMU   |      | ST HTS221        |        | ST MP34DT05      |    |
|     | 3-Axis Accel     |      | Temperature &    |        | Digital MEMS     |    |
|     | 3-Axis Gyro/Mag  |      | Humidity Sensor  |        | Microphone       |    |
|     +------------------+      +------------------+        +------------------+    |
|                                                                                   |
|  +--------------------+                                   +--------------------+  |
|  | Power Distribution |                                   | 2.4 GHz PCB        |  |
|  | MP2322 Step-Down   |                                   | Ceramic Antenna    |  |
|  | 5V Vin -> 3.3V VCC |                                   | (BLE 5.0 Beacon)   |  |
|  +--------------------+                                   +--------------------+  |
+-----------------------------------------------------------------------------------+
```

## 2. Mechanical Mounting on Industrial Machinery
To ensure accurate high-frequency vibration transmission:
- Mount the board rigidly to the motor bearing housing or pump casing using a machined aluminum bracket or cyanoacrylate mounting stud.
- Avoid foam or rubber dampening pads beneath the board, as they act as acoustic low-pass filters that attenuate bearing defect frequencies (> 50 Hz).
