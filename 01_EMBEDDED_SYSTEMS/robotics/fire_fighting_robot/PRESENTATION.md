# Executive Presentation Deck: Autonomous Fire Suppression Robot
**Organization:** Kernelwise Labs  
**Lead Systems Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Primary Silicon / Stack:** `ATmega328P / 3-Channel NIR Array / Submersible Pump`  
**Verification Status:** 100% Tests Passing • Zero Memory Leaks  

---

## Slide 1: Executive Title & System Overview
- **Project Title:** Autonomous Fire Suppression Robot
- **Subtitle:** Optical NIR Flame Triangulation & Directed Closed-Loop Extinguishing
- **Domain:** Emergency Robotics & Safety Systems
- **Author:** Aravind Reddy (Founder & Lead Architect, Kernelwise Labs)
- **Primary Platform:** ATmega328P / 3-Channel NIR Array / Submersible Pump
- **Verification Guarantee:** 100% Automated Unit Test Verification Suite Passing

---

## Slide 2: Problem Statement & Industrial Need
**Core Challenge:** Hazardous Human Exposure in Industrial Fire Emergencies
- Entering confined industrial facilities during active fires presents extreme thermal and toxic risks to personnel.
- Stationary sprinkler systems flood entire facilities indiscriminately, causing catastrophic water damage to unburnt equipment.
- Autonomous suppression units often suffer false positives caused by ambient sunlight or incandescent bulbs.
- Target Requirement: Wavelength-selective flame sensing, autonomous azimuth steering, and targeted water jetting.

---

## Slide 3: What We Built (Executive Solution)
**Deliverable Overview:** Tri-Directional Flame Triangulation & Articulated Suppression
- Engineered a 3-channel optical Near-Infrared (NIR) phototransistor array isolating open combustion wavelengths.
- Developed differential analog flux equations to compute flame source azimuth in real time.
- Integrated an isolated high-flow submersible pump nozzle with timed safety cutoff to prevent reservoir exhaustion.
- Deterministic FSM seamlessly transitions: PATROL, TRIANGULATION, APPROACH, EXTINGUISH, VERIFY.

---

## Slide 4: How We Built It (Architecture & Stack)
**System Topology:** High-Reliability Embedded Hardware & C++14 Stack
- Silicon: ATmega328P microcontroller managing sensor ADC channels and motor PWM.
- Optical Sensing: 3x narrow-angle NIR phototransistors (760nm–1100nm) calibrated for hydrocarbon flame emission.
- Suppression Hardware: 5V DC centrifugal submersible water pump driven via optocoupled relay.
- Verification: 10 unit test cases validating angular localization, approach thresholds, and pump cutoff limits.

---

## Slide 5: Why We Used These Tools & Devices (Design Justification)
**Design Tradeoffs & Rationale:** Safety Criticality & Sensor Selection Rationale
- Why 760nm–1100nm NIR: Specifically targets the radiant emission spectrum of carbon combustion, ignoring visible light.
- Why Tri-Channel Array: Provides 180° spatial coverage with simple analog differential math, requiring zero camera latency.
- Why Optocoupled Relays: Galvanically isolates motor and pump inductive back-EMF from the microcontroller core.
- Client Advantage: Rapid automated first-response intervention suppressing fires before structural escalation occurs.

---

## Slide 6: Project Operational Workflow
**Step-by-Step Functional Execution Lifecycle:**
- **01. Patrol & Scan:** Robot autonomously navigates room while monitoring 3 analog phototransistor channels.
- **02. Flame Localization:** Differential analog comparison (left vs. center vs. right) identifies flame heading vector.
- **03. Proportional Drive:** Differential steering aligns center sensor directly with radiant infrared peak.
- **04. Approach & Halt:** Robot approaches until flame intensity reaches target extinguishing distance, then locks brakes.
- **05. Suppression & Check:** Pump sprays targeted water jet for calibrated interval, verifying extinction before disengaging.

---

### Client Presentation Notes
*This presentation is generated to support client technical briefings, investor diligence reviews, and enterprise architecture consultations. For full source code, test suites, and IEEE research whitepapers, inspect the repository.*
