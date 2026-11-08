# Gesture Controlled Robotic Arm - Circuit Schematic & Power Routing

## 1. Schematic Flow & Power Distribution

```
   +-------------------------------+          +-------------------------------+
   | External 5.0V 3A Power Supply |          | Arduino UNO Logic / USB Power |
   +---------------+---------------+          +---------------+---------------+
                   |                                          |
       +-----------+-----------+                  +-----------+-----------+
       | (+) +5.0V             | (-) GND          | 5V Out                | GND
       v                       v                  v                       v
+---------------+      +---------------+   +---------------+      +---------------+
| Servo VCC Bus |      |   Common GND  |---| Arduino 5V    |      |  Arduino GND  |
+-------+-------+      +-------+-------+   +-------+-------+      +-------+-------+
        |                      |                   |                      |
        |                      |                   +-----------+          |
        |                      |                               |          |
        |                      |                               v          v
        |                      |                       +--------------------+
        |                      |                       |    MPU6050 IMU     |
        |                      |                       | VCC / GND / SCL/SDA|
        |                      |                       +----------+---------+
        |                      |                                  | (I2C Bus: A4/A5)
        |                      |                                  v
        |                      |                         [Arduino A4, A5]
        v                      v
+----------------------------------------------------+
|               4-DOF Joint Servos                   |
| Base (D3), Shoulder (D5), Elbow (D6), Gripper (D9) |
+----------------------------------------------------+
```

## 2. Shared Ground Bonding Requirement
> [!IMPORTANT]
> The negative terminal of the external 5V power supply **must be connected to the Arduino GND pin**. Without this common reference ground, PWM servo control signals will float, causing erratic servo jitter and loss of kinematic control.
