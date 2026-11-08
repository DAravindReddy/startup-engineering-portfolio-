# Microcontroller Pin Configuration & Mapping Table

**Target Silicon:** Microchip ATmega328P / Arduino UNO Rev3  
**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Complete Pin Mapping Table

| Pin No. | Port Pin | Direction | Signal Name | Connected Peripheral / Component | Electrical Signal Type | Hardware Notes |
|:---:|:---:|:---:|---|---|:---:|---|
| **0** | `PD0` | Input | `UART_RX` | USB Serial / Programmer | 5V TTL | Reserved for firmware upload & debug monitor. |
| **1** | `PD1` | Output | `UART_TX` | USB Serial / Programmer | 5V TTL | Reserved for firmware upload & debug monitor. |
| **2** | `PD2` | Input | `BT_RX` | HC-05 TXD Terminal | 3.3V / 5V TTL | SoftwareSerial RX; accepts direct 3.3V logic from HC-05. |
| **3** | `PD3` | Output | `BT_TX` | HC-05 RXD Terminal | Voltage Divided (3.3V) | SoftwareSerial TX; **must use 1kΩ / 2kΩ voltage divider** to step 5V down to 3.3V. |
| **4** | `PD4` | In/Out | `DHT11_DATA` | DHT11 1-Wire Data Terminal | Bidirectional 5V | Requires external 4.7kΩ to 10kΩ pull-up resistor to 5V. |
| **5** | `PD5` | Input | `SOIL_DIGITAL`| Soil Moisture Comparator Pin | Digital 5V | Optional digital comparator threshold output. |
| **6** | `PD6` | Output | `MOTOR_PWM_ENA` | L298N Motor Driver Enable A | PWM (Timer0, 980 Hz) | Variable speed control (0 - 255). |
| **7** | `PD7` | Output | `MOTOR_IN1` | L298N Direction Input 1 | Digital 5V | HIGH = Forward, LOW = Reverse. |
| **8** | `PB0` | Output | `MOTOR_IN2` | L298N Direction Input 2 | Digital 5V | Direction complementary pair. |
| **9** | `PB1` | Output | `SERVO_PWM` | SG90 Servo Signal Pin | PWM (Timer1, 50 Hz) | 1.0ms (0°) to 2.0ms (180°) standard pulse width. |
| **10**| `PB2` | Output | `RELAY_PUMP` | 1-Channel Relay Module IN | Digital 5V Active-LOW | LOW energizes relay (turns pump ON); HIGH de-energizes. |
| **11**| `PB3` | Output | `LED_APP1` | Appliance 1 / Room Light | Digital 5V via 220Ω | Visual indicator for Room Light L1. |
| **12**| `PB4` | Output | `LED_APP2` | Appliance 2 / Yard Light | Digital 5V via 220Ω | Visual indicator for Yard Light L2. |
| **13**| `PB5` | Output | `STATUS_LED` | Onboard Diagnostic LED | Digital 5V | 1 Hz heartbeat blink indicating cooperative loop health. |
| **A0**| `PC0` | Input | `SOIL_ANALOG` | Soil Moisture Analog Terminal | Analog 0V - 5V (10-bit) | Read by `analogRead()`. 0 = 0V, 1023 = 5V. |

---

## 2. Voltage Level Shifting for HC-05 Bluetooth

The HC-05 Bluetooth module operates internally at 3.3V CMOS levels. While its VCC terminal accepts 5V due to an onboard regulator, its RXD line is NOT 5V tolerant:

```text
Arduino Pin 3 (TX 5V) ----[ 1kΩ Resistor ]----+---- HC-05 RXD (3.3V Logic)
                                               |
                                        [ 2kΩ Resistor ]
                                               |
                                            GND (Common)
```

$$\text{V}_{\text{RXD}} = 5\text{V} \times \frac{2000}{1000 + 2000} = 3.33\text{V}$$
