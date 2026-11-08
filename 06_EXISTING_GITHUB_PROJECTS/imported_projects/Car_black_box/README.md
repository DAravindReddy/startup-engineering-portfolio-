# Automotive Event Data Recorder (Car Black Box)

[![Microcontroller: PIC16F877A](https://img.shields.io/badge/Microcontroller-PIC16F877A-blue.svg)](https://www.microchip.com/)
[![Compiler: MPLAB XC8](https://img.shields.io/badge/Compiler-MPLAB%20XC8-orange.svg)]()
[![Status: Completed](https://img.shields.io/badge/Status-Completed-brightgreen.svg)](PROJECT_STATUS.md)
[![Whitepaper](https://img.shields.io/badge/Whitepaper-IEEE%20Format-purple.svg)](whitepaper/whitepaper.md)
[![Original Source](https://img.shields.io/badge/Original%20Repo-My__projects-lightgrey.svg)](https://github.com/DAravindReddy/My_projects)

An automotive event monitoring and telemetry black box developed using the **Microchip PIC16F877A microcontroller**. The system records driving events (gear shifts, collisions, speed violations) with timestamps into non-volatile EEPROM and provides display, authentication, and serial telemetry download facilities.

---

## 1. System Features
- **Real-Time Driving Dashboard:** Displays live 24-hour time (`HH:MM:SS`), transmission gear status, and speed (0 - 99 km/h) on a 16x2 Character LCD.
- **Event Logging Pipeline:** Logs critical vehicular events (Ignition `ON`, Neutral `GN`, Reverse `GR`, Forward Gears `G1`-`G4`, Braking `BR`, Collision `C_`).
- **Non-Volatile Crash-Proof Storage:** Serializes 10-byte event records to an external I2C 24C02 EEPROM circular ring buffer.
- **Real-Time Clock:** Interfaces with a battery-backed DS1307 RTC over I2C.
- **PIN-Protected Administrative Menu:** Requires 4-digit security PIN (`1010`) to view, download, or clear logs.
- **Automated Security Lockout:** Implements a 60-second hardware timer lockout upon 3 consecutive invalid authentication attempts.
- **UART Diagnostic Dump:** Downloads complete event history to PC terminal at 9600 baud.

---

## 2. Hardware Architecture & Pinout

```mermaid
graph LR
    PIC["PIC16F877A (20 MHz)"]
    PIC -->|PORTD/PORTE| CLCD["16x2 LCD Display"]
    PIC -->|PORTB| Keypad["Digital Keypad (Switches)"]
    PIC -->|RA5 (AN4)| Pot["Speed Sensor Simulator"]
    PIC <-->|RC3/RC4 (I2C)| RTC["DS1307 Real-Time Clock"]
    PIC <-->|RC3/RC4 (I2C)| EEPROM["24C02 Serial EEPROM"]
    PIC -->|RC6/RC7 (UART)| PC["PC Terminal (9600 Baud)"]
```

| Component | PIC16F877A Pins | Function |
|---|---|---|
| 16x2 Character LCD | `RD0-RD3` (Data), `RE0-RE2` (Control) | 4-bit display interface |
| DS1307 RTC | `RC3` (SCL), `RC4` (SDA) | I2C timestamping (4.7kΩ pull-ups) |
| 24C02 EEPROM | `RC3` (SCL), `RC4` (SDA) | I2C non-volatile storage |
| Speed Potentiometer | `RA5 / AN4` | 10-bit analog conversion |
| Digital Keypad | `RB0 - RB5` | User switches (Collision, Gears, Menu) |
| UART Serial | `RC6` (TX), `RC7` (RX) | 9600 baud telemetry log download |

---

## 3. Documentation Index

- [Bill of Materials & Components List](hardware/components_list.md)
- [Microcontroller Pin Configuration](hardware/pin_configuration.md)
- [Circuit Schematic Diagram](hardware/circuit_diagram/circuit_schematic.md)
- [Hardware Interconnection Guide](hardware/hardware_connections.md)
- [Technical Architecture Specification](architecture/system_architecture.md)
- [State Machine & Data Flow](architecture/data_flow.md)
- [Software Design & EEPROM Memory Map](documentation/software_design.md)
- [User Operating Manual](documentation/user_manual.md)
- [Troubleshooting Fault Matrix](documentation/troubleshooting.md)
- [Formal Research Whitepaper](whitepaper/whitepaper.md)

---

## 4. Attribution & Original Repository
- **Author:** **Aravind Reddy** ([@DAravindReddy](https://github.com/DAravindReddy))
- **Original Repository:** [`https://github.com/DAravindReddy/My_projects`](https://github.com/DAravindReddy/My_projects)
- **License:** [MIT License](LICENSE)
