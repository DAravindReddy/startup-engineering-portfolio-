# Hardware Connections & Setup Guide

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Step-by-Step Interconnect Procedures

### 1.1 Bluetooth HC-05 Connection
1. Connect `HC-05 VCC` to Arduino `5V`.
2. Connect `HC-05 GND` to Arduino `GND`.
3. Connect `HC-05 TXD` to Arduino `Digital Pin 2`.
4. Connect Arduino `Digital Pin 3` to one leg of a `1kΩ resistor`.
5. Connect the other leg of the `1kΩ resistor` to `HC-05 RXD` and one leg of a `2kΩ resistor`.
6. Connect the second leg of the `2kΩ resistor` to `GND`.

### 1.2 DHT11 Temperature & Humidity Sensor
1. Connect `DHT11 Pin 1 (VCC)` to Arduino `5V`.
2. Connect `DHT11 Pin 2 (DATA)` to Arduino `Digital Pin 4`.
3. Connect a `10kΩ pull-up resistor` between `DHT11 VCC` and `DATA`.
4. Connect `DHT11 Pin 4 (GND)` to Arduino `GND` (Pin 3 is NC).

### 1.3 Soil Moisture Probe
1. Connect `VCC` of the sensor comparator board to Arduino `5V`.
2. Connect `GND` of the comparator board to Arduino `GND`.
3. Connect `A0 (Analog Out)` of the comparator board to Arduino `Analog Pin A0`.
4. Insert probe prongs firmly into plant root zone soil.

### 1.4 L298N DC Motor Driver
1. Connect `12V Power` to L298N `12V Terminal`.
2. Connect `Power Ground` to L298N `GND Terminal` AND tie to Arduino `GND`.
3. Connect L298N `ENA` to Arduino `Digital Pin 6 (PWM)`.
4. Connect L298N `IN1` to Arduino `Digital Pin 7`.
5. Connect L298N `IN2` to Arduino `Digital Pin 8`.
6. Connect DC motor wires to L298N `OUT1` and `OUT2`.

### 1.5 SG90 Servo Actuator
1. Connect `Brown wire (GND)` to System Ground.
2. Connect `Red wire (VCC)` to External Regulated `5V`.
3. Connect `Orange wire (Signal)` to Arduino `Digital Pin 9 (PWM)`.

### 1.6 Relay & Submersible Water Pump
1. Connect Relay `VCC` to `5V` and `GND` to Ground.
2. Connect Relay `IN` to Arduino `Digital Pin 10`.
3. Wire the positive DC pump lead through the Relay `Common (COM)` and `Normally Open (NO)` terminals.

---

## 2. Pre-Power Checkout Checklist

- [ ] Common ground verified between Arduino, 12V power supply, and external 5V rail.
- [ ] Voltage divider on HC-05 RXD verified with multimeter (must not exceed 3.4V).
- [ ] No short circuits between 5V and GND rails.
- [ ] Arduino USB cable disconnected during initial high-voltage motor power connection.
