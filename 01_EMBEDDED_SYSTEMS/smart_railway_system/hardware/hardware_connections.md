# Smart Railway Track System - Physical Hardware Connections & Assembly Guide

## 1. Physical Component Placement

1. **Dual IR Crack Sensors**: Mount the two TCRT5000 infrared reflective sensor heads beneath the trolley frame pointing vertically downward at each rail crown (Left Rail and Right Rail). Maintain an optical standoff distance between $3\text{ mm} - 6\text{ mm}$ above the rail surface.
2. **Obstacle Ultrasonic Sonar**: Fasten the HC-SR04 ultrasonic rangefinder directly along the front center of the trolley aimed along the longitudinal track axis.
3. **Signal Light Mast**: Mount the 3-aspect LED signal head (Green, Yellow, Red) on a miniature wayside mast beside the level crossing.
4. **Crossing Barrier Gate**: Mount the SG90 micro-servo horizontally beside the railway roadway intersection, attaching a lightweight barrier arm to its horn.

---

## 2. Electrical Wiring Checklist

- [ ] **Dual IR Crack Sensors to Arduino**:
  - Left Sensor `DO` $\rightarrow$ Arduino `D2`
  - Right Sensor `DO` $\rightarrow$ Arduino `D3`
  - Both Sensors `VCC` $\rightarrow$ Arduino `5V`
  - Both Sensors `GND` $\rightarrow$ Arduino `GND`
- [ ] **HC-SR04 Ultrasonic Sonar**:
  - `Trig` $\rightarrow$ Arduino `D4`
  - `Echo` $\rightarrow$ Arduino `D5`
  - `VCC` $\rightarrow$ Arduino `5V`
  - `GND` $\rightarrow$ Arduino `GND`
- [ ] **Level Crossing Barrier Gate Servo**:
  - `Signal (Orange)` $\rightarrow$ Arduino `D6` (PWM)
  - `VCC (Red)` $\rightarrow$ `5V`
  - `GND (Brown)` $\rightarrow$ `GND`
- [ ] **Multi-Aspect Signal Mast**:
  - Green LED Anode $\rightarrow$ 220Ω $\rightarrow$ Arduino `D7`
  - Yellow LED Anode $\rightarrow$ 220Ω $\rightarrow$ Arduino `D8`
  - Red LED Anode $\rightarrow$ 220Ω $\rightarrow$ Arduino `D9`
  - All LED Cathodes $\rightarrow$ `GND`
- [ ] **Alarm Buzzer**:
  - Positive lead $(+)$ $\rightarrow$ Arduino `D10`
  - Negative lead $(-)$ $\rightarrow$ `GND`
