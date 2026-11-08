# Software Design Specification

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Concurrency Model: Cooperative Multitasking

The ATmega328P microcontroller operates on a single core at 16 MHz without a preemptive real-time operating system (RTOS). To satisfy hard timing deadlines across communication and sensor sampling without race conditions, execution is structured around cooperative time-sliced polling:

```mermaid
stateDiagram-v2
    [*] --> Idle_Poll
    Idle_Poll --> Process_BT_Stream : Serial Available
    Process_BT_Stream --> Idle_Poll : Buffer Ingested / Command Executed

    Idle_Poll --> Poll_Soil_Moisture : now - last_soil >= 1000ms
    Poll_Soil_Moisture --> Solve_Irrigation_FSM
    Solve_Irrigation_FSM --> Idle_Poll

    Idle_Poll --> Poll_DHT11 : now - last_dht >= 2000ms
    Poll_DHT11 --> Idle_Poll

    Idle_Poll --> Update_Hardware_Pins : Output State Mutation
    Update_Hardware_Pins --> Idle_Poll

    Idle_Poll --> Broadcast_Telemetry : now - last_telemetry >= 3000ms
    Broadcast_Telemetry --> Idle_Poll
```

---

## 2. Irrigation Finite State Machine (FSM)

```mermaid
stateDiagram-v2
    [*] --> IDLE

    state IDLE {
        description: "Pump OFF. Monitoring soil moisture."
    }
    state WATERING {
        description: "Pump ON. Delivering water. Tracking elapsed time."
    }
    state COOLDOWN {
        description: "Pump OFF. Allowing water percolation (5000ms)."
    }
    state SAFETY_TRIP {
        description: "EMERGENCY CUTOFF. Pump locked OFF."
    }

    IDLE --> WATERING : Soil Moisture < Dry_Threshold (35%)
    WATERING --> COOLDOWN : Soil Moisture >= Wet_Threshold (65%)
    WATERING --> SAFETY_TRIP : Elapsed Run Time >= 15,000ms
    COOLDOWN --> IDLE : Elapsed Cooldown >= 5,000ms
    SAFETY_TRIP --> IDLE : Operator Manual Reset (PUMP_OFF)
```
