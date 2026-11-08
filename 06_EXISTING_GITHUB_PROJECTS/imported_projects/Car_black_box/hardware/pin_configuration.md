# Microcontroller Pin Configuration: PIC16F877A Car Black Box

**Microcontroller:** Microchip PIC16F877A (40-Pin DIP Package)  
**Clock:** 20 MHz (HS Mode)  

---

## 1. Pin Configuration Table

| Pin No. | PIC Port Pin | Direction | Functional Signal | Connected Hardware Peripheral | Protocol / Functionality |
|:---:|:---:|:---:|---|---|---|
| **1** | `MCLR#/VPP` | Input | Master Clear | Pull-up 10kΩ to VDD + Reset Button | Hardware reset and ICSP programming voltage. |
| **2** | `RA0/AN0` | Input | Unused / Reserved | Analog Input Header | Auxiliary analog channel. |
| **7** | `RA5/AN4` | Input | `ADC_SPEED` | 10kΩ Potentiometer Wiper | Speedometer sensor simulation (0V = 0 km/h, 5V = 99 km/h). |
| **8** | `RE0/RD` | Output | `LCD_RS` | 16x2 CLCD Register Select Pin | LOW = Instruction Register, HIGH = Data Register. |
| **9** | `RE1/WR` | Output | `LCD_RW` | 16x2 CLCD Read/Write Pin | Tied to LOW (Write Only) or dynamically controlled. |
| **10**| `RE2/CS` | Output | `LCD_EN` | 16x2 CLCD Enable Strobe Pin | High-to-Low transition latches 4-bit nibble. |
| **11, 32**| `VDD` | Power | `+5V` | Regulated 5V Power Supply | Core VDD power rail. |
| **12, 31**| `VSS` | Power | `GND` | System Ground | Core VSS ground rail. |
| **13**| `OSC1/CLKI` | Input | `XTAL_IN` | 20 MHz Crystal + 22 pF to GND | High-speed primary clock oscillator. |
| **14**| `OSC2/CLKO` | Output | `XTAL_OUT` | 20 MHz Crystal + 22 pF to GND | Oscillator feedback output. |
| **18**| `RC3/SCK/SCL`| Output | `I2C_SCL` | DS1307 RTC & 24C02 EEPROM SCL | I2C Serial Clock (requires 4.7kΩ pull-up to 5V). |
| **23**| `RC4/SDI/SDA`| In/Out | `I2C_SDA` | DS1307 RTC & 24C02 EEPROM SDA | I2C Serial Data bidirectional (requires 4.7kΩ pull-up). |
| **25**| `RC6/TX/CK` | Output | `UART_TX` | USB-UART Converter RX Pin | Serial transmit @ 9600 baud for log download. |
| **26**| `RC7/RX/DT` | Input | `UART_RX` | USB-UART Converter TX Pin | Serial receive @ 9600 baud. |
| **19 - 22**| `RD0 - RD3` | Output | `LCD_DATA` | 16x2 CLCD Data Pins D4 - D7 | 4-bit parallel data transmission bus. |
| **33 - 38**| `RB0 - RB5` | Input | `KEYPAD_SW` | Digital Keypad Switches (SW1 - SW6) | User inputs for Collision, Gear UP/DOWN, Menu, and Password. |
