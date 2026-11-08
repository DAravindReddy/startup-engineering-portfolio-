# Arduino Nano 33 BLE Sense AI - Hardware Connections & Mounting Guide

## 1. Electrical Power & Interfacing

1. **Standalone Deployment (Wireless Beacon Mode)**:
   - Connect a single-cell LiPo battery (3.7V, 500mAh - 2000mAh) or USB power bank to the `VUSB` / `VIN` pin and `GND`.
   - The onboard high-efficiency switching regulator (MP2322) steps down input voltages from 4.5V - 21.0V to 3.3V logic.
2. **Industrial Interlock Relay (Optional Output)**:
   - Connect Arduino pin `D3` to the logic trigger input of an isolated 3.3V/5V industrial solid-state relay (SSR).
   - Wire the SSR output contacts in series with the machine's Emergency Stop (E-Stop) safety loop to automatically trip the motor contactor when `MACHINE_STATE_CRITICAL_FAULT` occurs.

---

## 2. Sensor Alignment & Vibration Axis Mapping

- **X-Axis**: Aligned parallel to the short edge of the Nano board (radial vibration).
- **Y-Axis**: Aligned parallel to the long edge of the Nano board (axial machine thrust vibration).
- **Z-Axis**: Perpendicular to the board plane (normal radial vibration & static 1G gravity).
- Vector magnitude calculation ($\sqrt{a_x^2 + a_y^2 + a_z^2}$) guarantees rotational orientation invariance regardless of mounting angle.
