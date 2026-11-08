# Functional Requirements Traceability Matrix: Car Black Box

**Project:** Automotive Event Data Recorder  

---

| Req ID | Functional Requirement | Implementing Source | Verification Method | Status |
|:---:|---|---|---|:---:|
| **CBB-01** | Real-time vehicle dashboard displaying Time (`HH:MM:SS`), Event (`EV`), and Speed (`SP`). | `car_black_box_def.c` | LCD display observation & ADC sweep. | **VERIFIED** |
| **CBB-02** | Automatic event logging on gear shifts, braking, and collision impacts. | `car_black_box_def.c`, `main.c` | Keypad simulation and EEPROM byte read. | **VERIFIED** |
| **CBB-03** | Battery-backed real-time clock timestamping down to the second. | `ds1307.c`, `i2c.c` | Oscilloscope I2C trace & time audit. | **VERIFIED** |
| **CBB-04** | Persistent storage of up to 10 historical driving events in non-volatile EEPROM. | `Eeprom.c`, `i2c.c` | Power cycle verification test. | **VERIFIED** |
| **CBB-05** | Secure 4-digit PIN access for administrative menu and log review. | `car_black_box_def.c` | Password brute force lockout test. | **VERIFIED** |
| **CBB-06** | On-screen scrollable event log viewer on 16x2 Character LCD. | `car_black_box_def.c`, `clcd.c` | Menu navigation test. | **VERIFIED** |
| **CBB-07** | High-speed serial UART log extraction to PC terminal @ 9600 baud. | `uart.c`, `car_black_box_def.c` | Terminal capture verification. | **VERIFIED** |
| **CBB-08** | In-system password re-programming persisted to EEPROM. | `car_black_box_def.c` | New PIN authentication test. | **VERIFIED** |
| **CBB-09** | In-system RTC time setting and synchronization. | `car_black_box_def.c`, `ds1307.c` | Clock synchronization test. | **VERIFIED** |
| **CBB-10** | One-touch log memory purge and index reset. | `car_black_box_def.c` | Memory audit post-clear. | **VERIFIED** |
