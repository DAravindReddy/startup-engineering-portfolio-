# Executive Presentation Deck: TinyML Mahalanobis Anomaly Detector
**Organization:** Kernelwise Labs  
**Lead Systems Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Primary Silicon / Stack:** `Nordic nRF52840 (64MHz ARM Cortex-M4F) / LSM9DS1 IMU`  
**Verification Status:** 100% Tests Passing • Zero Memory Leaks  

---

## Slide 1: Executive Title & System Overview
- **Project Title:** TinyML Mahalanobis Anomaly Detector
- **Subtitle:** On-Device Inertial Machine Health Inference & BLE 5.0 GATT Telemetry
- **Domain:** TinyML, Edge Computing & Predictive AI
- **Author:** Aravind Reddy (Founder & Lead Architect, Kernelwise Labs)
- **Primary Platform:** Nordic nRF52840 (64MHz ARM Cortex-M4F) / LSM9DS1 IMU
- **Verification Guarantee:** 100% Automated Unit Test Verification Suite Passing

---

## Slide 2: Problem Statement & Industrial Need
**Core Challenge:** Cloud Telemetry Bottlenecks in Rotating Machinery Monitoring
- Transmitting high-frequency vibration data (100Hz+) to cloud servers overwhelms industrial Wi-Fi and cellular bandwidth.
- Cloud-dependent machine anomaly detection introduces intolerable latencies (seconds) during catastrophic mechanical failure.
- Deep learning models (CNNs/RNNs) are too computationally heavy to run on low-power battery-operated edge microcontrollers.
- Target Requirement: Sub-millisecond on-device anomaly scoring, <2KB RAM footprint, and multi-year battery life.

---

## Slide 3: What We Built (Executive Solution)
**Deliverable Overview:** On-Device Mahalanobis TinyML on ARM Cortex-M4F
- Implemented covariance-whitened Mahalanobis distance calculation running directly on an ARM Cortex-M4F microcontroller.
- Extracts 3-axis vibration magnitude, variance, and kurtosis features in real time without cloud roundtrips.
- Inference completes in <1.2ms using hardware floating-point acceleration (FPU) with <1.5KB RAM utilization.
- Broadcasts anomalous machine health flags over standard BLE 5.0 GATT Environmental Sensing profiles.

---

## Slide 4: How We Built It (Architecture & Stack)
**System Topology:** Silicon TinyML & Bluetooth 5.0 GATT Stack
- Silicon: Nordic Semiconductor nRF52840 SoC (64MHz ARM Cortex-M4F, 256KB RAM, 1MB Flash).
- Inertial Sensing: STMicroelectronics LSM9DS1 9-axis motion sensor sampled over SPI.
- Math Engine: Embedded C++14 matrix math pipeline utilizing hardware FPU instructions.
- Verification: 14 unit test cases proving statistical convergence and numerical stability of on-chip covariance.

---

## Slide 5: Why We Used These Tools & Devices (Design Justification)
**Design Tradeoffs & Rationale:** Mathematical Elegance & Silicon Efficiency
- Why Mahalanobis Distance: Accounts for feature correlations and scales variance without needing heavy neural network weights.
- Why nRF52840: Integrated BLE 5.0 radio and hardware FPU enable combined DSP inference and wireless reporting on a single chip.
- Why BLE 5.0 GATT: Enables direct interoperability with existing industrial handheld scanners and edge gateways.
- Client Advantage: Zero cloud hosting costs, zero network bandwidth consumption, and immediate microsecond alert dispatch.

---

## Slide 6: Project Operational Workflow
**Step-by-Step Functional Execution Lifecycle:**
- **01. High-Rate IMU Ingestion:** LSM9DS1 samples 3-axis vibration at 100Hz into ring buffer via high-speed SPI bus.
- **02. Feature Transformation:** Computes windowed mean amplitude, variance, and kurtosis to capture abnormal harmonics.
- **03. Covariance Whitening:** Multiplies feature vector by pre-calibrated baseline inverse covariance matrix using hardware FPU.
- **04. Anomaly Evaluation:** Calculates Mahalanobis distance in <1.2ms; compares result against empirical critical threshold.
- **05. BLE GATT Broadcast:** Updates BLE 5.0 GATT characteristic, transmitting machine health status and alert to factory supervisor.

---

### Client Presentation Notes
*This presentation is generated to support client technical briefings, investor diligence reviews, and enterprise architecture consultations. For full source code, test suites, and IEEE research whitepapers, inspect the repository.*
