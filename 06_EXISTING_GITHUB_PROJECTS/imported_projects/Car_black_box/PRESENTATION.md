# Executive Presentation Deck: Automotive Safety Event Logger (Car Black Box)
**Organization:** Kernelwise Labs  
**Lead Systems Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Primary Silicon / Stack:** `Microchip PIC16F877A / 24C02 I2C EEPROM / DS1307 RTC`  
**Verification Status:** 100% Tests Passing • Zero Memory Leaks  

---

## Slide 1: Executive Title & System Overview
- **Project Title:** Automotive Safety Event Logger (Car Black Box)
- **Subtitle:** PIC16F877A Flight Recorder, Non-Volatile EEPROM & RTC Telemetry
- **Domain:** Automotive Embedded Systems & Functional Safety
- **Author:** Aravind Reddy (Founder & Lead Architect, Kernelwise Labs)
- **Primary Platform:** Microchip PIC16F877A / 24C02 I2C EEPROM / DS1307 RTC
- **Verification Guarantee:** 100% Automated Unit Test Verification Suite Passing

---

## Slide 2: Problem Statement & Industrial Need
**Core Challenge:** Loss of Critical Crash Telemetry During Severe Vehicle Collisions
- Automotive accident investigations require forensic telemetry (speed, gear, collision signals) stamped with sub-second accuracy.
- Violent collisions frequently sever vehicle battery power instantly, wiping volatile RAM logs before they can be saved.
- Consumer microcontrollers lack hardware brownout protection and non-volatile memory endurance.
- Target Requirement: Sub-second RTC timestamps, brownout-safe EEPROM persistence, and supervisor access security.

---

## Slide 3: What We Built (Executive Solution)
**Deliverable Overview:** Automotive Event Data Recorder with Non-Volatile Persistence
- Engineered an automotive flight recorder on the Microchip PIC16F877A microcontroller using MPLAB XC8.
- Implemented non-volatile crash event persistence utilizing an external I2C 24C02 EEPROM surviving sudden battery loss.
- Integrated an I2C DS1307 real-time clock logging precise timestamped vehicle speed and transmission gear changes.
- Constructed a 16x2 Character LCD interface and password-protected UART console for supervisor log offloading.

---

## Slide 4: How We Built It (Architecture & Stack)
**System Topology:** Automotive PIC Microcontroller & Dual I2C Bus Stack
- Silicon: Microchip PIC16F877A 8-bit RISC microcontroller executing embedded C compiled with MPLAB XC8.
- Peripherals: Maxim DS1307 battery-backed RTC, Atmel 24C02 I2C EEPROM (256-byte circular log), 16x2 CLCD.
- Telemetry Ingestion: On-chip ADC measuring simulated speedometer voltage; digital interrupt lines tracking gear changes.
- Documentation: Formally published in 13-section IEEE Research Whitepaper KW-WP-02.

---

## Slide 5: Why We Used These Tools & Devices (Design Justification)
**Design Tradeoffs & Rationale:** Automotive Hardening & Forensic Reliability
- Why PIC16F877A: Automotive-grade silicon with built-in Brown-Out Reset (BOR) preventing memory corruption during voltage sags.
- Why 24C02 I2C EEPROM: Retains critical crash records for 40+ years without power; byte-level writes complete in <5ms.
- Why DS1307 RTC: Battery backup guarantees continuous sub-second timekeeping regardless of main battery disconnects.
- Client Advantage: Meets insurance and legal forensic audit standards for commercial vehicle fleet monitoring.

---

## Slide 6: Project Operational Workflow
**Step-by-Step Functional Execution Lifecycle:**
- **01. Continuous Sampling:** PIC microcontroller samples speed ADC and gear switch inputs every 100ms.
- **02. Event Detection:** Detects state change (gear shift, collision trigger, overspeed threshold) and queries DS1307 RTC via I2C.
- **03. Non-Volatile Write:** Atomically writes timestamp, event ID, and speed into circular 24C02 EEPROM log address in <5ms.
- **04. Dashboard Display:** Renders real-time vehicle speed, gear, and event counter on 16x2 Character LCD.
- **05. Forensic Offloading:** Supervisor authenticates via password to dump crash history over UART to forensic PC application.

---

### Client Presentation Notes
*This presentation is generated to support client technical briefings, investor diligence reviews, and enterprise architecture consultations. For full source code, test suites, and IEEE research whitepapers, inspect the repository.*
