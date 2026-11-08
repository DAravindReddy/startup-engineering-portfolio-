# Object Avoiding Robot - Technical Documentation

## 1. System Overview

The **Autonomous Object Avoiding Robot** is a mobile robotic platform capable of real-time obstacle detection and collision avoidance in unknown indoor environments. Built upon an Arduino UNO (ATmega328P) microcontroller, an HC-SR04 ultrasonic rangefinder mounted on an SG90 micro-servo pan turret, and an L298N dual H-bridge motor driver, the system demonstrates robust autonomous navigation without human intervention.

---

## 2. Ultrasonic Sonar Physics & Echo Processing

The HC-SR04 ultrasonic sensor transmits an 8-cycle ultrasonic burst at $40\text{ kHz}$ upon receiving a $10\ \mu\text{s}$ TTL high trigger pulse. The sensor raises its `ECHO` output pin high for a duration proportional to the time-of-flight ($\Delta t_{\text{TOF}}$) taken for the acoustic wavefront to bounce off an obstacle and return.

Distance $D$ in centimeters is derived as:

$$D = \frac{\Delta t_{\text{TOF}} \times v_{\text{sound}}}{2}$$

Where $v_{\text{sound}} \approx 343\text{ m/s} = 0.0343\text{ cm}/\mu\text{s}$ at $20^\circ\text{C}$:

$$D = \frac{\Delta t_{\text{TOF}}}{58.2}\quad [\text{cm}]$$

### Acoustic Filtering
To eliminate transient multipath reflections and acoustic echo nulls, the driver implements a 2-tap exponential moving average (EMA) filter:

$$D_{\text{filtered}}[k] = 0.70 \cdot D_{\text{raw}}[k] + 0.30 \cdot D_{\text{filtered}}[k-1]$$

---

## 3. Locomotion & Differential Steering Kinematics

The robot utilizes a two-wheel differential drive mechanism with a passive forward caster wheel. Steering is governed by speed differentials between the left ($v_L$) and right ($v_R$) drive wheels:

- **Forward Cruise:** $v_L = v_R = v_{\text{cruise}}$
- **Differential Left Pivot:** $v_L = -v_{\text{turn}}, \quad v_R = +v_{\text{turn}}$
- **Differential Right Pivot:** $v_L = +v_{\text{turn}}, \quad v_R = -v_{\text{turn}}$
- **Emergency Reverse:** $v_L = v_R = -v_{\text{creep}}$

The yaw angular rate $\omega_z$ during a differential turn with wheel baseline $W = 14.5\text{ cm}$ and wheel radius $R = 3.3\text{ cm}$ is:

$$\omega_z = \frac{R \cdot (\omega_R - \omega_L)}{W}$$
