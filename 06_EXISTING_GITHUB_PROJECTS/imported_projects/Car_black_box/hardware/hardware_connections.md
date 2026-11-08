# Hardware Interconnections & Board Assembly: Car Black Box

**Project:** Automotive Event Data Recorder (Car Black Box)  

---

## 1. Assembly & Wiring Guidelines

### 1.1 Power Supply Rails
- Regulate vehicle or battery input down to 5.0V using an LM7805 or switching buck regulator.
- Tie PIC16F877A Pins 11 and 32 to `+5V (VDD)`.
- Tie PIC16F877A Pins 12 and 31 to `GND (VSS)`.
- Place 100 nF ceramic bypass capacitors directly across VDD and VSS pins.

### 1.2 20 MHz Crystal Oscillator Circuit
- Connect a 20 MHz quartz crystal between Pin 13 (`OSC1`) and Pin 14 (`OSC2`).
- Connect a 22 pF ceramic capacitor from Pin 13 to GND, and another 22 pF capacitor from Pin 14 to GND.
- Ensure crystal leads are routed with minimal trace length to avoid parasitic capacitance.

### 1.3 I2C Bus Wiring (DS1307 RTC & 24C02 EEPROM)
- Both the DS1307 RTC and 24C02 EEPROM are open-drain I2C devices.
- Connect Pin 18 (`RC3/SCL`) to SCL of both ICs.
- Connect Pin 23 (`RC4/SDA`) to SDA of both ICs.
- Place two **4.7kΩ pull-up resistors** from the SCL and SDA lines to `+5V`.
- Connect the 3V CR2032 coin cell positive terminal to Pin 3 (`VBAT`) of the DS1307, and negative terminal to GND.

### 1.4 16x2 Character LCD (4-bit Mode)
- `VSS` (Pin 1) to GND, `VDD` (Pin 2) to +5V.
- `V0` (Pin 3) to 10kΩ potentiometer wiper for contrast tuning.
- `RS` (Pin 4) to PIC `RE0`.
- `RW` (Pin 5) to PIC `RE1` (or GND for write-only).
- `EN` (Pin 6) to PIC `RE2`.
- Data Pins `D4, D5, D6, D7` to PIC `RD0, RD1, RD2, RD3`.
- Backlight Anode `A` (Pin 15) to +5V via 220Ω resistor; Cathode `K` (Pin 16) to GND.

### 1.5 Digital Keypad
- Connect switches SW1 through SW6 to PIC PORTB (`RB0 - RB5`) configured with internal weak pull-ups or external 10kΩ pull-up resistors.
