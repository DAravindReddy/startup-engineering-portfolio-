# Gesture Controlled Robotic Arm - System Architecture

## 1. Architectural Overview

The Gesture Controlled 4-DOF Robotic Arm architecture maps continuous human hand gesture dynamics into multi-axis robotic manipulator poses. The pipeline integrates 6-axis inertial perception, complementary orientation filtering, spatial coordinate mapping, and velocity-limited joint actuation.

```mermaid
flowchart TD
    subgraph GlovePerception ["Gesture Perception Glove"]
        IMU["MPU6050 6-Axis IMU\n(3-Axis Accel + 3-Axis Gyro)"]
        GripBtn["Tactile Grip Trigger\n(Active-LOW Pushbutton)"]
    end

    subgraph MotionFiltering ["Signal Conditioning & Orientation"]
        I2C["I2C Transceiver Core (400 kHz)"]
        CompFilter["Complementary Orientation Filter\n(High-Pass Gyro + Low-Pass Accel, α=0.96)"]
        AngleOut["Filtered Roll & Pitch (deg)"]
    end

    subgraph KinematicsCore ["Kinematic Translation Core"]
        MapEngine["KinematicsMapper\n(Roll -> Base Pan, Pitch -> Shoulder Lift)"]
        HorizComp["Horizon Articulation Engine\n(Elbow Coordination & Horizon Keeping)"]
        TargetAngles["Target Joint Vector (θ_base, θ_shldr, θ_elbw, θ_grp)"]
    end

    subgraph MotionActuation ["Joint Servo Motion Profiler"]
        SlewLimiter["Velocity Slew-Rate Limiter\n(Max 90 deg/s Acceleration Cap)"]
        PWMGen["Hardware Timers / PWM Waveform Generators"]
        Servos["4-DOF Manipulator Servos\n(MG996R & SG90 Joint Actuators)"]
    end

    IMU -->|Raw I2C Accel/Gyro| I2C
    I2C --> CompFilter
    CompFilter --> AngleOut
    AngleOut --> MapEngine

    GripBtn -->|Binary Clamp Signal| MapEngine
    MapEngine --> HorizComp
    HorizComp --> TargetAngles

    TargetAngles --> SlewLimiter
    SlewLimiter --> PWMGen
    PWMGen --> Servos
```

---

## 2. 4-DOF Manipulator Kinematic Joint Formulation

```mermaid
graph LR
    Base["Joint 1: Base Pan\n(θ_1: 10° - 170°)\nAxis: Z-vertical"]
    Shoulder["Joint 2: Shoulder Pitch\n(θ_2: 20° - 160°)\nAxis: Y-horizontal"]
    Elbow["Joint 3: Elbow Pitch\n(θ_3: 15° - 165°)\nAxis: Y-horizontal"]
    Gripper["Joint 4: End-Effector\n(θ_4: 40° - 115°)\nClaw Clamp"]

    Base --> Shoulder
    Shoulder --> Elbow
    Elbow --> Gripper
```
