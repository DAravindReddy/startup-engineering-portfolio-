# Technical Documentation: Automotive Event Data Recorder (Car Black Box)

**Microcontroller:** Microchip PIC16F877A  
**Toolchain:** Microchip MPLAB X IDE / XC8 Compiler  
**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  

---

## 1. System Overview

The **Automotive Event Data Recorder (Car Black Box)** is an embedded crash-survivable telemetry instrument designed to log vehicular driving dynamics. Implemented on the 8-bit Harvard RISC PIC16F877A microcontroller operating at 20 MHz, the system records vehicle speed, operational transmission events, and exact real-time clock timestamps into an external I2C non-volatile EEPROM.

---

## 2. Low-Level Subsystem Implementation

### 2.1 I2C Bus Master Subsystem (`i2c.c` / `i2c.h`)
The PIC16F877A hardware Synchronous Serial Port (MSSP) is configured in I2C Master mode:
- Standard 100 kHz bit rate generated via baud rate generator register `SSPADD = 0x31` (with $F_{\text{OSC}} = 20\text{ MHz}$).
- Hardware start condition (`SEN`), restart (`RSEN`), stop (`PEN`), and acknowledge sequences (`ACKEN`, `ACKSTAT`) are managed with busy-wait flag synchronization on `SSPIF`.

### 2.2 Real-Time Clock Interfacing (`ds1307.c` / `ds1307.h`)
- Connects to a Maxim/Dallas DS1307 serial RTC at 7-bit slave address `0x68` (Write: `0xD0`, Read: `0xD1`).
- Operates in Binary-Coded Decimal (BCD) mode. Registers `0x00`, `0x01`, and `0x02` encode Seconds (00-59), Minutes (00-59), and Hours (00-23 in 24-hour mode).
- The driver reads time registers in a single sequential I2C burst and performs BCD-to-ASCII conversion:

$$\text{ASCII High} = \left(\frac{\text{reg} \gg 4}{10}\right) + \text{'0'}, \quad \text{ASCII Low} = (\text{reg} \ \& \ 0\text{x0F}) + \text{'0'}$$

### 2.3 Non-Volatile Memory Storage (`Eeprom.c` / `Eeprom.h`)
- Leverages an external AT24C02 256-byte I2C EEPROM at slave address `0xA0`.
- Memory is organized into a circular ring buffer containing up to 10 event logs.
- Each event log consumes exactly 10 bytes:
  - Bytes 0–1: Hours (ASCII)
  - Bytes 2–3: Minutes (ASCII)
  - Bytes 4–5: Seconds (ASCII)
  - Bytes 6–7: Event Code (e.g., `GN`, `GR`, `G1`-`G4`, `C_`, `BR`)
  - Bytes 8–9: Speed in km/h (ASCII `00` - `99`)
- Base address for log entry $k \in [0, 9]$ is computed as:

$$\text{EEPROM Address} = k \times 10$$

### 2.4 Speed Measurement Subsystem (`adc.c` / `adc.h`)
- 10-bit Successive Approximation ADC configured on channel AN4 (pin RA5).
- Conversion clock derived from $F_{\text{OSC}} / 32$.
- Result registers `ADRESH:ADRESL` (Right Justified) yield raw value $V_{\text{raw}} \in [0, 1023]$.
- Speed $S \in [0, 99\text{ km/h}]$ is derived via linear interpolation:

$$S = \left\lfloor \frac{V_{\text{raw}} \times 99}{1023} \right\rfloor$$

### 2.5 Display and Keypad Drivers (`clcd.c`, `digital_keypad.c`)
- **16x2 Character LCD:** Operates in 4-bit nibble mode on PORTD (`RD0-RD3` data, `RE0-RE2` control), minimizing pin count.
- **Keypad:** Level-detection and state-change detection modes with software switch debouncing on PORTB (`RB0-RB5`).
