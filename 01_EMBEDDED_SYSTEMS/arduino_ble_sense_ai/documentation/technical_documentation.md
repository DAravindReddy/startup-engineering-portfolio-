# Arduino Nano 33 BLE Sense AI - Technical Documentation

## 1. System Overview

The **Arduino Nano 33 BLE Sense AI** is an industrial edge TinyML sensor node designed for condition-based predictive maintenance (PdM) of electric motors, industrial pumps, gearboxes, and rotating machinery. Running directly on an ultra-low-power Nordic Semiconductor nRF52840 ARM Cortex-M4F microcontroller, the system samples 3-axis mechanical vibration at 100 Hz, extracts statistical time-domain features across rolling 64-sample windows, performs unsupervised anomaly inference, and broadcasts machine health telemetry over Bluetooth Low Energy (BLE) GATT services.

---

## 2. Vibration Signal Conditioning & Feature Mathematics

Mechanical degradation in rotating machinery (e.g., ball bearing spalling, gear tooth pitting, unbalance, and misalignment) induces distinct harmonic vibration patterns:

1. **Root Mean Square (RMS):** Measures overall dynamic kinetic vibration energy:
   $$\text{RMS} = \sqrt{\frac{1}{N} \sum_{i=1}^N \left(a_i - \bar{a}\right)^2}$$
2. **Peak-to-Peak Amplitude:** Captures maximum mechanical impact excursion:
   $$A_{\text{pk-pk}} = \max_{i}(a_i) - \min_{i}(a_i)$$
3. **Crest Factor:** Ratio of peak excursion to RMS, serving as an early indicator of localized bearing race impact shocks:
   $$C_f = \frac{A_{\text{peak}}}{\text{RMS}}$$

---

## 3. Bluetooth Low Energy GATT Architecture

The node implements a custom BLE Peripheral GATT profile:

| UUID | Name | Permissions | Data Type | Description |
|:---|:---|:---:|:---:|:---|
| `...1214` | Machine Health Service | Service | 128-bit Custom | Primary service container |
| `...0001` | Anomaly Score | Read / Notify | Float32 | Continuous failure risk (0.0% - 100.0%) |
| `...0002` | Vibration RMS | Read / Notify | Float32 | Real-time vibration energy in $\text{m/s}^2$ |
| `...0003` | Temperature | Read / Notify | Float32 | Operating temperature in ${^\circ}\text{C}$ |
| `...0004` | Machine State | Read / Notify | Uint8 | $0 = \text{Normal}, 1 = \text{Warning}, 2 = \text{Fault}$ |
