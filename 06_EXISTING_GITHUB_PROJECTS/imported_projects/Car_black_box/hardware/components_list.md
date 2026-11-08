# Bill of Materials (BOM) & Hardware Specifications: Car Black Box

**Project:** Automotive Event Data Recorder (Car Black Box)  
**Microcontroller Target:** Microchip PIC16F877A (28/40-Pin Enhanced Flash Microcontroller)  
**Author / Maintainer:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  

---

## 1. Component Specification Table

| Item | Component | Part Number / Model | Operating Voltage | Current | Interfacing Bus | Role / Functional Scope |
|:---:|---|---|:---:|:---:|:---:|---|
| 1 | Microcontroller | PIC16F877A | 4.5V - 5.5V DC | 25 mA @ 20MHz | 8-bit RISC Core | Central supervisor, peripheral control, menu navigation, non-volatile logging. |
| 2 | Crystal Oscillator | 20 MHz Quartz Crystal | - | - | OSC1 / OSC2 Pins | High-frequency primary system clock (5 MIPS instruction rate). |
| 3 | Load Capacitors | 22 pF Ceramic (x2) | 50V | - | Crystal to GND | Clock oscillation stability. |
| 4 | Real-Time Clock | DS1307 Serial RTC | 4.5V - 5.5V DC | 200 µA (VCC), 500 nA (VBAT)| I2C (RC3 SCL, RC4 SDA) | Non-volatile timestamp generator (Seconds, Minutes, Hours, Date, Month, Year). |
| 5 | RTC Backup Battery | CR2032 3V Lithium Coin Cell | 3.0V DC | 225 mAh | VBAT Pin | Maintains timekeeper during vehicular ignition-off states. |
| 6 | Non-Volatile EEPROM| AT24C02 / 24C04 Serial EEPROM | 4.5V - 5.5V DC | 1 mA (Active), 1 µA (Standby) | I2C (RC3 SCL, RC4 SDA) | 2048-bit (256-byte) non-volatile circular event log storage. |
| 7 | Vehicle Display | 16x2 Character LCD (CLCD) | 5.0V DC | 1.5 mA (Logic), 20 mA (Backlight) | 4-bit Parallel (PORTD/PORTE) | Real-time display of Gear, Speed, Time, Menu, and Log viewer. |
| 8 | Operator Keypad | Digital Keypad (Switches SW1-SW6)| 5.0V DC | Pull-down / Pull-up | PORTB Pins | Driver event inputs (Collision, Gear up/down) and password navigation. |
| 9 | Speed Sensor Sim | 10kΩ Linear Potentiometer | 0V - 5V DC | 0.5 mA | Analog ADC Channel AN4 (RA5) | Vehicle speed simulation (0 to 99 km/h). |
| 10 | Serial Transceiver | MAX232 / USB-UART CP2102 | 3.3V - 5.0V DC | 15 mA | USART (RC6 TX, RC7 RX) | Off-board log telemetry extraction to PC terminal @ 9600 baud. |
| 11 | Power Supply | 7805 Linear 5V Regulator | 7V - 18V In | 1.0A Max | TO-220 Package | Automotive 12V battery step-down to clean 5.0V logic rail. |
| 12 | Decoupling Caps | 100 nF Ceramic + 10 µF Electrolytic| 25V | - | VDD to VSS | Power rail transient suppression. |
