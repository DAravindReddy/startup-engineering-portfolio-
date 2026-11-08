# Hardware & Firmware Troubleshooting Guide: Car Black Box

**Project:** Automotive Event Data Recorder  

---

## 1. Diagnostic Matrix

| Problem / Fault | Root Cause | Remediation Procedure |
|---|---|---|
| **CLCD shows black boxes or no text** | Contrast pin `V0` misadjusted or LCD initialization failed. | Adjust the 10kΩ potentiometer on pin 3 (`V0`). Ensure 4-bit initialization delay (>15ms after power-on) is respected. |
| **Time stuck at 00:00:00 or frozen** | DS1307 oscillator stopped (CH bit set) or missing I2C pull-ups. | Verify 32.768 kHz watch crystal on DS1307 pins 1 and 2. Ensure Bit 7 of register `0x00` (`CH` bit) is written to `0` during initialization. Verify 4.7kΩ pull-ups on SCL/SDA. |
| **System hangs when accessing EEPROM** | I2C bus NACK or stuck line. | Check 24C02 hardware address pins (`A0, A1, A2` must be tied to GND for address `0xA0`). Ensure clock line transitions. |
| **Speed potentiometer reads 0 at all times** | ADC configuration error or open pin RA5. | Verify `ADCON1` register configuration for analog channel AN4. Measure voltage on RA5 (0V to 5V). |
| **UART download emits garbage characters** | Baud rate mismatch or incorrect oscillator frequency. | Ensure PC serial terminal is set to 9600 baud, 8 data bits, no parity, 1 stop bit (8-N-1). Verify 20 MHz crystal and `SPBRG = 129` (High Baud rate mode). |
