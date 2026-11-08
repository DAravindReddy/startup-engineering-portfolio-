# Fire Fighting Robot - Physical Connections & Assembly Guide

## 1. Physical Component Placement

1. **Flame Sensor Mounting**: Install the 3 infrared flame sensor modules along the front chassis edge:
   - **Left Sensor**: Angled approximately $45^\circ$ to the left.
   - **Center Sensor**: Aimed directly along the robot's forward centerline ($0^\circ$).
   - **Right Sensor**: Angled approximately $45^\circ$ to the right.
2. **Extinguisher Nozzle & Pump**:
   - Secure the 250ml reservoir securely in the chassis center of mass to prevent tipping.
   - Mount the mini submersible pump inside the reservoir.
   - Direct the outlet silicone tube through a front-mounted nozzle aimed directly forward.
3. **Relay Board**: Mount the isolated 5V relay module away from sensitive analog/RF lines to isolate inductive switching noise.

---

## 2. Electrical Wiring Checklist

- [ ] **Flame Sensor DO to Arduino**:
  - Left Sensor `DO` $\rightarrow$ Arduino `D2`
  - Center Sensor `DO` $\rightarrow$ Arduino `D3`
  - Right Sensor `DO` $\rightarrow$ Arduino `D4`
  - All Sensor `VCC` $\rightarrow$ Arduino `5V`
  - All Sensor `GND` $\rightarrow$ Arduino `GND`
- [ ] **L298N Motor Driver to Arduino**:
  - `ENA` $\rightarrow$ `D5`
  - `IN1` $\rightarrow$ `D6`
  - `IN2` $\rightarrow$ `D7`
  - `IN3` $\rightarrow$ `D8`
  - `IN4` $\rightarrow$ `D9`
  - `ENB` $\rightarrow$ `D10`
- [ ] **Relay & Water Pump**:
  - Relay `IN` $\rightarrow$ Arduino `D11`
  - Relay `VCC` $\rightarrow$ `5V`
  - Relay `GND` $\rightarrow$ `GND`
  - Battery/Power Rail $\rightarrow$ Relay `COM`
  - Relay `NO` $\rightarrow$ Water Pump `(+)`
  - Water Pump `(-)` $\rightarrow$ Battery `GND`
- [ ] **Alarm Buzzer**:
  - Anode `(+)` $\rightarrow$ Arduino `D13`
  - Cathode `(-)` $\rightarrow$ `GND`
