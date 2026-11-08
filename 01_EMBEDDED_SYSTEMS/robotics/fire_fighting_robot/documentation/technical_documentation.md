# Fire Fighting Robot - Technical Documentation

## 1. System Overview

The **Autonomous Fire Fighting Robot** is an edge robotics system designed to detect, locate, approach, and suppress localized fire outbreaks autonomously. Equipped with an optical 3-channel infrared flame detector array, a dual H-bridge differential locomotion system, and a high-flow submersible water pump governed by an ATmega328P microcontroller, the system provides unmanned early fire suppression for industrial facilities, storage depots, and laboratory environments.

---

## 2. Infrared Flame Sensing Physics

Hydrocarbon flames emit optical radiation across the electromagnetic spectrum, with distinct spectral emission peaks in the near-infrared (NIR) band between $760\text{ nm}$ and $1100\text{ nm}$ due to hot water vapor ($\text{H}_2\text{O}$) and carbon dioxide ($\text{CO}_2$) transitions.

The robot's flame detection array utilizes silicon phototransistors reverse-biased into a high-sensitivity configuration:
- **Optical Filter:** Black epoxy resin encapsulation blocks visible ambient illumination while admitting wavelengths between $760\text{ nm} - 1100\text{ nm}$.
- **Signal Conditioning:** An onboard LM393 dual comparator converts the analog photo-current into a clean digital signal against an adjustable multi-turn potentiometer threshold.
- **Output Logic:** When flame radiation impinges on the photodiode, resistance collapses, driving the comparator output to TTL `LOW` (Active-LOW signal).

---

## 3. Extinguishing Fluid Dynamics & Relay Safety

The water suppression subsystem uses a 5V/6V DC mini centrifugal submersible pump capable of delivering up to $1.2\text{ L/min}$ at $0.4\text{ bar}$ head pressure:
- **Relay Driver:** An optocoupled 5V relay module isolates the inductive flyback and electromagnetic interference (EMI) of the motor coil from the microcontroller logic rail.
- **Timed Burst Duration:** Spraying is executed in discrete $3.0\text{ s}$ bursts (`EXTINGUISH_SPRAY_MS`) to conserve water reservoir volume and prevent localized flooding.
- **Watchdog Protection:** A software watchdog timer automatically forces the relay open if an unexpected software lockup occurs, preventing motor coil burnout.
