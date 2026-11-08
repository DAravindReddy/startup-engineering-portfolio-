# Object Avoiding Robot - Data Flow & Component Architecture

## 1. Information Data Flow

```mermaid
sequenceDiagram
    autonumber
    participant MCU as ATmega328P Core
    participant US as HC-SR04 Sonar
    participant Servo as SG90 Turret
    participant Motors as L298N Motors

    MCU->>Motors: Cruise Forward (PWM: 190)
    loop Continuous Sonar Sampling
        MCU->>US: Trigger 10μs Pulse
        US-->>MCU: Echo Pulse Width (Time-of-Flight)
        MCU->>MCU: Calculate Distance & Filter Sample
    end

    Note over MCU: Forward Obstacle <= 30cm Detected
    MCU->>Motors: Stop (PWM: 0)
    MCU->>Servo: Rotate to 160° (Left)
    Note over MCU,Servo: Wait 250ms Mechanical Settle
    MCU->>US: Ping Sonar
    US-->>MCU: dist_left = 65cm

    MCU->>Servo: Rotate to 20° (Right)
    Note over MCU,Servo: Wait 250ms Mechanical Settle
    MCU->>US: Ping Sonar
    US-->>MCU: dist_right = 18cm

    MCU->>Servo: Recenter to 90°
    Note over MCU: dist_left (65cm) > dist_right (18cm)
    MCU->>Motors: Pivot Left (PWM: 170)
    Note over MCU,Motors: Pivot 350ms (~60° heading shift)
    MCU->>Motors: Resume Forward Cruise (PWM: 190)
```

---

## 2. Component Software Architecture

- **`config.h`**: Hardware pinouts, timer presets, distance thresholds (`DISTANCE_CRITICAL_CM = 15`, `DISTANCE_WARNING_CM = 30`).
- **`motor_driver.h/.cpp`**: Abstracted differential drive primitives (`DRIVE_FORWARD`, `DRIVE_REVERSE`, `DRIVE_PIVOT_LEFT`, `DRIVE_PIVOT_RIGHT`, `DRIVE_STOP`).
- **`ultrasonic_sensor.h/.cpp`**: Sonar pulse trigger and exponential moving average (EMA) noise filter.
- **`servo_scanner.h/.cpp`**: 0° - 180° pan turret servo control.
- **`navigation_fsm.h/.cpp`**: Non-blocking finite state machine evaluating obstacle threats and scheduling transitions.
