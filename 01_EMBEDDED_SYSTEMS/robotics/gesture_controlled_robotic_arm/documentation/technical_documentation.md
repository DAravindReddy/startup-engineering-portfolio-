# Gesture Controlled Robotic Arm - Technical Documentation

## 1. System Overview

The **Gesture Controlled 4-DOF Robotic Arm** is an intuitive teleoperation robotic platform that translates natural human hand gestures into synchronized multi-axis robotic movements. By reading hand tilt and angular rates from an MPU6050 6-axis Inertial Measurement Unit (IMU) mounted on a wearable operator glove, the embedded microcontroller executes real-time sensor fusion and kinematic joint translation to actuate an articulated 4-degree-of-freedom mechanical arm with zero jerk.

---

## 2. IMU Sensor Fusion & Complementary Filtering

Accelerometer measurements are reliable in the long term because they reference the static gravity vector $\mathbf{g}$, but they are corrupted in the short term by external kinetic accelerations and vibration. Conversely, rate gyroscopes provide clean short-term angular rate integration but suffer from progressive low-frequency integration drift.

To synthesize an optimal estimate of pitch ($\theta$) and roll ($\phi$), the firmware employs a digital Complementary Filter:

$$\phi[k] = \alpha \cdot \left(\phi[k-1] + \omega_x[k] \cdot \Delta t\right) + (1 - \alpha) \cdot \arctan\left(\frac{a_y}{\sqrt{a_x^2 + a_z^2}}\right)$$

$$\theta[k] = \alpha \cdot \left(\theta[k-1] + \omega_y[k] \cdot \Delta t\right) + (1 - \alpha) \cdot \arctan\left(\frac{-a_x}{\sqrt{a_y^2 + a_z^2}}\right)$$

Where:
- $\alpha = 0.96$ is the filter blending coefficient.
- $\Delta t = 0.02\text{ s}$ is the sampling period (50 Hz).
- $\omega_x, \omega_y$ are the gyroscope angular velocities.
- $a_x, a_y, a_z$ are raw gravitational accelerometer components.

---

## 3. Kinematic Mapping & Joint Velocity Slew Limiter

Directly applying raw target angles to high-torque servos (MG996R) causes violent mechanical acceleration spikes, inducing structural vibration, excessive current surges (> 2.5A per motor), and premature nylon/metal gear teeth stripping.

The firmware integrates an automated **Slew-Rate Velocity Limiter**:

$$\theta_{\text{joint}}[k] = \theta_{\text{joint}}[k-1] + \text{clamp}\left(\theta_{\text{target}}[k] - \theta_{\text{joint}}[k-1], -\Delta \theta_{\max}, +\Delta \theta_{\max}\right)$$

Where:
- $\Delta \theta_{\max} = \omega_{\max} \cdot \Delta t = 90^\circ/\text{s} \times 0.02\text{ s} = 1.8^\circ/\text{tick}$.
- Any abrupt operator hand movement is smoothly profile-stepped, guaranteeing continuous trajectory curvature and extended actuator lifespan.
