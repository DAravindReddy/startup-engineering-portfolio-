# Fire Fighting Robot - System Architecture

## 1. Architectural Overview

The Autonomous Fire Fighting Robot operates on a hierarchical, event-driven control loop. The perception pipeline continuously polls the 3-channel optical flame sensor array, the decision core calculates angular heading adjustments, and the actuation layer commands differential wheel motors and high-pressure extinguishing relays.

```mermaid
flowchart TD
    subgraph Perception ["Infrared Flame Perception Subsystem"]
        FS_L["Left Flame Module\n(45° FOV)"]
        FS_C["Center Flame Module\n(0° Forward)"]
        FS_R["Right Flame Module\n(45° FOV)"]
        Aggregator["Sensor Array Aggregator\n(Sampling & Debounce)"]
    end

    subgraph DecisionCore ["Autonomous Decision Core (Arduino UNO)"]
        FSM["Fire Fighting State Machine\n(PATROL, ALIGN, APPROACH, EXTINGUISH, REVERIFY)"]
        SafetyWatchdog["Pump Protection Watchdog\n(Max 3 Sprays / 5.0s Timeout)"]
        AlarmGen["Strobe & Acoustic Alarm Controller"]
    end

    subgraph Actuation ["Locomotion & Extinguisher Actuation"]
        HBridge["L298N Dual H-Bridge Driver"]
        Relay["Optocoupled 5V Relay"]
        Motors["TT Geared DC Drive Motors"]
        Pump["Submersible DC Water Pump"]
    end

    FS_L --> Aggregator
    FS_C --> Aggregator
    FS_R --> Aggregator
    Aggregator -->|Flame Position & Status| FSM

    FSM --> SafetyWatchdog
    SafetyWatchdog -->|Pump Gate Signal| Relay
    Relay -->|Switched VCC| Pump

    FSM -->|Direction & PWM| HBridge
    HBridge --> Motors

    FSM --> AlarmGen
```

---

## 2. Navigation & Extinguishing State Automaton

```mermaid
stateDiagram-v2
    [*] --> FF_STATE_PATROL_SEARCH

    FF_STATE_PATROL_SEARCH --> FF_STATE_ALIGN_LEFT: Flame on Left
    FF_STATE_PATROL_SEARCH --> FF_STATE_ALIGN_RIGHT: Flame on Right
    FF_STATE_PATROL_SEARCH --> FF_STATE_APPROACH_FIRE: Flame Centered

    FF_STATE_ALIGN_LEFT --> FF_STATE_APPROACH_FIRE: Flame Aligned to Center
    FF_STATE_ALIGN_RIGHT --> FF_STATE_APPROACH_FIRE: Flame Aligned to Center

    FF_STATE_APPROACH_FIRE --> FF_STATE_EXTINGUISH_SPRAY: Extinguishing Proximity Reached

    FF_STATE_EXTINGUISH_SPRAY --> FF_STATE_REVERIFY: Spray Burst Timeout (3.0s)

    FF_STATE_REVERIFY --> FF_STATE_PATROL_SEARCH: Fire Extinguished (FLAME_NONE)
    FF_STATE_REVERIFY --> FF_STATE_EXTINGUISH_SPRAY: Fire Still Active & Sprays < 3
    FF_STATE_REVERIFY --> FF_STATE_FAULT_HALT: Fire Still Active & Sprays >= 3

    FF_STATE_FAULT_HALT --> [*]: Requires Manual Reset & Refill
```
