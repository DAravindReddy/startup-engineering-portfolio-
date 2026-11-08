# Object Avoiding Robot - System Architecture

## 1. Architectural Overview

The Autonomous Object Avoiding Robot is an embedded real-time robotics system operating on an event-driven, non-blocking Finite State Machine (FSM). The system continuously assesses forward obstacle clearance, initiates pan-tilt sonar sweeps upon approaching spatial obstacles, evaluates alternative trajectory paths, and commands differential steering actuators to maneuver around collisions.

```mermaid
flowchart TD
    subgraph Sensing ["Perception & Sensing Subsystem"]
        HC["HC-SR04 Ultrasonic Sonar"]
        ServoTurret["SG90 Panning Turret\n(Center 90°, Left 160°, Right 20°)"]
    end

    subgraph Controller ["Decision & Scheduling Core (Arduino UNO)"]
        EchoFilter["Echo Time-of-Flight Filter\n(Outlier rejection & EMA)"]
        FSM["Navigation Finite State Machine\n(CRUISE, HALT, SCAN, DECIDE, PIVOT, REVERSE)"]
        Scheduler["Non-Blocking Timer Scheduler\n(millis() tick generator)"]
    end

    subgraph Actuation ["Locomotion & Actuation Subsystem"]
        HBridge["L298N Dual H-Bridge Driver\n(Directional Inverters & PWM Gates)"]
        LeftMotor["Left TT Geared DC Motor"]
        RightMotor["Right TT Geared DC Motor"]
    end

    HC -->|Echo Pulse Width| EchoFilter
    EchoFilter -->|Filtered Clearance (cm)| FSM
    Scheduler -->|Tick 20ms| FSM

    FSM -->|Target Pan Angle| ServoTurret
    FSM -->|Direction & PWM Speed| HBridge
    HBridge -->|OUT1/OUT2| LeftMotor
    HBridge -->|OUT3/OUT4| RightMotor
```

---

## 2. Finite State Machine Dynamics

```mermaid
stateDiagram-v2
    [*] --> NAV_STATE_CRUISE

    NAV_STATE_CRUISE --> NAV_STATE_HALT: Forward Obstacle <= 30cm
    NAV_STATE_CRUISE --> NAV_STATE_REVERSE: Immediate Danger <= 15cm

    NAV_STATE_HALT --> NAV_STATE_SCAN_LEFT: Stop Motors, Pan 160°
    NAV_STATE_SCAN_LEFT --> NAV_STATE_SCAN_RIGHT: Record Left Clearance, Pan 20°
    NAV_STATE_SCAN_RIGHT --> NAV_STATE_DECIDE_PATH: Record Right Clearance, Center 90°

    NAV_STATE_DECIDE_PATH --> NAV_STATE_PIVOT_LEFT: dist_left >= dist_right
    NAV_STATE_DECIDE_PATH --> NAV_STATE_PIVOT_RIGHT: dist_right > dist_left
    NAV_STATE_DECIDE_PATH --> NAV_STATE_REVERSE: Both sides < 30cm (Trapped)

    NAV_STATE_PIVOT_LEFT --> NAV_STATE_CRUISE: Turn Duration (350ms) Expired
    NAV_STATE_PIVOT_RIGHT --> NAV_STATE_CRUISE: Turn Duration (350ms) Expired
    NAV_STATE_REVERSE --> NAV_STATE_PIVOT_RIGHT: Reverse Duration (300ms) Expired
```
