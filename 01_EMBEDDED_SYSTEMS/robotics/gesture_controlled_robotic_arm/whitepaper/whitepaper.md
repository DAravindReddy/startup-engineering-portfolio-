# Intuitive Teleoperation of Articulated 4-DOF Robotic Manipulators via Inertial Glove Gesture Tracking and Slew-Rate Controlled Kinematics

**Author:** Aravind Reddy  
**Affiliation:** Kernelwise Labs  
**Date:** March 2026  
**Document ID:** KW-WP-2026-ARM-001  
**Target Venue:** IEEE Transactions on Human-Machine Systems / IEEE Robotics and Automation Letters  

---

## Abstract
Teleoperation of multi-axis articulated robotic arms in explosive ordnance disposal, hazardous biological material handling, and prosthetic rehabilitation typically relies on cumbersome multi-axis joysticks or teach pendants. These conventional input devices impose high cognitive workloads on operators due to non-intuitive Cartesian-to-joint mapping and lack of natural kinesthetic alignment. This paper presents an intuitive teleoperation framework that maps natural hand orientation directly to a 4-degree-of-freedom (4-DOF) articulated robotic manipulator using a wearable 6-axis inertial measurement unit (IMU). By coupling a complementary attitude estimation filter ($\alpha = 0.96$) with coordinated joint angle transformations and a velocity slew-rate limiter ($90^\circ/\text{s}$), the system achieves fluid, jerk-free trajectory following while protecting servo gearboxes from mechanical shock and excessive current transients. Experimental evaluation over 50 pick-and-place trials demonstrates a task completion time reduction of $38.4\%$ compared to dual-joystick teleoperation, with an end-effector trajectory following error under $2.4^\circ$ and zero actuator stalls, proving its viability for low-cost, human-in-the-loop robotic manipulation.

**Index Terms**—Robotic Teleoperation, Human-Machine Interface, Inertial Measurement Unit, Complementary Filter, 4-DOF Manipulator, Slew-Rate Limiter, Wearable Glove.

---

## 1. Introduction
Remotely operated robotic manipulators are vital tools in environments that pose lethal risks to human operators, including nuclear decommissioning, underwater pipeline servicing, high-voltage electrical line repair, and surgical assistance. In these domains, the human operator provides high-level cognitive scene understanding, spatial reasoning, and obstacle avoidance, while the mechanical manipulator executes the physical grasping task.

However, standard industrial interfaces—such as dual-axis joysticks, keyboard directional controls, or teach pendants—suffer from severe human-factors deficiencies:
1. **High Cognitive Burden:** Mapping 2D joystick deflections to 4-DOF or 6-DOF 3D manipulator joints requires continuous mental coordinate transformations.
2. **Delayed Reaction Times:** Switching between joint selection modes on a pendant introduces operator latency.
3. **Mechanical Shock:** Sudden joystick reversals command step changes in target PWM duty cycles, creating torque spikes that strip reduction gearboxes and cause high motor back-EMF resets.

This paper develops a wearable inertial gesture control glove that measures natural human hand gestures in real time and smoothly maps them into coordinated multi-axis manipulator joint angles.

---

## 2. Mathematical Modeling of Hand Orientation
The operator glove mounts an InvenSense MPU6050 containing a 3-axis MEMS accelerometer ($\mathbf{a} = [a_x, a_y, a_z]^T$) and a 3-axis rate gyroscope ($\boldsymbol{\omega} = [\omega_x, \omega_y, \omega_z]^T$).

### 2.1 Gravitational Roll and Pitch Formulation
From static accelerometer projections:

$$\phi_{\text{acc}} = \arctan\left(\frac{a_y}{\sqrt{a_x^2 + a_z^2}}\right)$$

$$\theta_{\text{acc}} = \arctan\left(\frac{-a_x}{\sqrt{a_y^2 + a_z^2}}\right)$$

### 2.2 Complementary Filter Fusion
To suppress dynamic vibration without the heavy matrix inversion overhead of an extended Kalman filter (EKF) on an 8-bit ATmega328P microcontroller, we employ a digital Complementary Filter:

$$\phi[k] = \alpha \cdot \left(\phi[k-1] + \omega_x[k] \cdot \Delta t\right) + (1 - \alpha) \cdot \phi_{\text{acc}}[k]$$

$$\theta[k] = \alpha \cdot \left(\theta[k-1] + \omega_y[k] \cdot \Delta t\right) + (1 - \alpha) \cdot \theta_{\text{acc}}[k]$$

With blending factor $\alpha = 0.96$ and update rate $f = 50\text{ Hz}$ ($\Delta t = 0.02\text{ s}$), high-frequency gyro drift and low-frequency acceleration noise are simultaneously eliminated.

---

## 3. Kinematic Coordinate Mapping
The 4-DOF manipulator joints comprise:
- **Joint 1 (Base Pan $\theta_1$):** Driven by hand roll $\phi \in [-50^\circ, +50^\circ]$, linearly mapped to $[10^\circ, 170^\circ]$.
- **Joint 2 (Shoulder Pitch $\theta_2$):** Driven by hand pitch $\theta \in [-45^\circ, +45^\circ]$, mapped to $[20^\circ, 160^\circ]$.
- **Joint 3 (Elbow Pitch $\theta_3$):** Articulated inversely with the shoulder ($\theta_3 = 180^\circ - \theta_2$) to maintain end-effector horizon leveling.
- **Joint 4 (Gripper $\theta_4$):** Switched between $40^\circ$ (Open) and $115^\circ$ (Clamped) via tactile finger trigger.

---

## 4. Slew-Rate Motion Profiling & Actuator Protection
Servomotors under open-loop position control attempt to reach step targets at infinite instantaneous acceleration. The resulting inrush current ($I_{\text{stall}} \approx 1.5\text{ A}$ per MG996R servo) induces severe supply rail brownouts.

The system enforces a maximum rotational velocity constraint $\omega_{\max} = 90^\circ/\text{s}$:

$$\theta_i[k] = \theta_i[k-1] + \text{sign}(\theta_{i,\text{target}} - \theta_i[k-1]) \cdot \min\left(|\theta_{i,\text{target}} - \theta_i[k-1]|, \ \omega_{\max} \cdot \Delta t\right)$$

This mathematical constraint guarantees bounded acceleration, smooth visual tracking, and eliminates mechanical back-lash ringing.

---

## 5. Experimental Evaluation & Results

### 5.1 Test Methodology
A standardized pick-and-place task (grasping a 40mm wooden block from a pickup station, elevating it 120mm, rotating 90°, and releasing it into a target receptacle) was executed across 50 trials comparing:
1. Traditional Dual-Axis Joystick Interface.
2. The Proposed IMU Gesture Teleoperation Interface.

### 5.2 Performance Benchmarks

| Metric | Traditional Joystick | Proposed Gesture Interface | Performance Delta |
|:---|:---:|:---:|:---:|
| **Mean Task Completion Time** | $18.2\text{ s} \pm 2.4\text{ s}$ | $11.2\text{ s} \pm 1.1\text{ s}$ | **-38.4% faster** |
| **Target Acquisition Attempts** | $2.8 \pm 0.6$ attempts | $1.2 \pm 0.4$ attempts | **-57.1% fewer adjustments** |
| **Peak Servo Inrush Current** | $2.85\text{ A}$ | $1.42\text{ A}$ | **-50.2% current reduction** |
| **Tracking Latency** | $45\text{ ms}$ | $22\text{ ms}$ | **-51.1% latency reduction** |
| **Grip Success Rate (50 runs)**| $84.0\%$ (42/50) | **96.0%** (48/50) | **+14.3% reliability** |

---

## 6. Conclusion
This paper validated an intuitive, low-latency gesture-controlled teleoperation framework for multi-axis robotic arms. By merging IMU orientation filtering with coordinated kinematic mapping and velocity slew limiting, the system substantially lowers operator cognitive load while preserving mechanical actuator integrity. Future research will explore 6-DOF bilateral haptic force feedback and computer-vision assisted auto-grasping.

---

## 7. References
1. T. B. Sheridan, *Telerobotics, Automation, and Human Supervisory Control*, MIT Press, 1992.
2. J. Craig, *Introduction to Robotics: Mechanics and Control*, 3rd ed., Pearson, 2005.
3. R. Mahony, T. Hamel, and J.-M. Pflimlin, "Nonlinear complementary filters on the special orthogonal group," *IEEE Transactions on Automatic Control*, vol. 53, no. 5, pp. 1203-1218, 2008.
