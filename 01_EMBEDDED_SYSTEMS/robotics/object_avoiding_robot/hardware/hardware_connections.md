# Object Avoiding Robot - Physical Hardware Connections & Assembly Guide

## 1. Mechanical Assembly Sequence

1. **Chassis Preparation**: Secure the two TT geared DC motors to the acrylic chassis base plate using M3 machine screws and motor brackets.
2. **Caster Wheel Installation**: Mount the front omnidirectional caster wheel to the underside of the chassis using standoffs.
3. **Wheels Attachment**: Press the rubberized drive wheels firmly onto the dual-flat TT motor output shafts.
4. **Driver & MCU Placement**: Fasten the L298N motor driver board and Arduino UNO onto the top deck using nylon standoffs.
5. **Servo Turret Assembly**: Fix the SG90 micro-servo horn to the front center of the chassis pointing forward. Mount the ultrasonic sensor bracket onto the servo horn.

---

## 2. Electrical Wiring Checklist

- [ ] **L298N to Left Motor**: Connect `OUT1` and `OUT2` screw terminals to the Left TT Motor leads.
- [ ] **L298N to Right Motor**: Connect `OUT3` and `OUT4` screw terminals to the Right TT Motor leads.
- [ ] **L298N Logic Inputs to Arduino**:
  - `ENA` $\rightarrow$ Arduino `D5` (Remove jumper from ENA header to enable PWM speed).
  - `IN1` $\rightarrow$ Arduino `D6`
  - `IN2` $\rightarrow$ Arduino `D7`
  - `IN3` $\rightarrow$ Arduino `D8`
  - `IN4` $\rightarrow$ Arduino `D9`
  - `ENB` $\rightarrow$ Arduino `D10` (Remove jumper from ENB header to enable PWM speed).
- [ ] **HC-SR04 Sonar Sensor to Arduino**:
  - `VCC` $\rightarrow$ Arduino `5V`
  - `GND` $\rightarrow$ Arduino `GND`
  - `Trig` $\rightarrow$ Arduino `D11`
  - `Echo` $\rightarrow$ Arduino `D12`
- [ ] **SG90 Servo to Arduino**:
  - `Red (Power)` $\rightarrow$ `5V` rail
  - `Brown (GND)` $\rightarrow$ `GND` rail
  - `Orange (Signal)` $\rightarrow$ Arduino `D3` (PWM)
- [ ] **Ground Bonding**: Ensure Arduino `GND` and L298N `GND` are bonded with a low-resistance jumper wire.
