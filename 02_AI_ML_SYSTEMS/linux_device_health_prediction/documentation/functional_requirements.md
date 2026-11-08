# Linux Device Health Prediction AI - Functional Requirements Specification

## 1. System Scope & Objectives

The primary objective of the Linux Device Health Prediction AI is to provide proactive, autonomous operational monitoring of Linux-based edge computers, robotics control boards, single-board computers (SBCs), and enterprise servers, detecting hardware and software anomalies before catastrophic outages occur.

---

## 2. Functional Requirements

### FR-01: Kernel Telemetry Acquisition
- **FR-01.1**: The system shall periodically parse `/proc/stat` to calculate user CPU, system CPU, iowait percentage, and context switch frequency.
- **FR-01.2**: The system shall sample `/proc/loadavg` for 1-minute, 5-minute, and 15-minute load averages.
- **FR-01.3**: The system shall read `/proc/meminfo` to capture total, available, cached, buffer, and swap memory allocations.
- **FR-01.4**: The system shall monitor thermal zones via `/sys/class/thermal/thermal_zone*/temp` and track cooling governor states in `/sys/class/thermal/cooling_device*/cur_state`.
- **FR-01.5**: The system shall sample network interface packet drop counters via `/sys/class/net/*/statistics/`.
- **FR-01.6**: When executed in non-Linux or simulated environments, the collector shall provide synthetic telemetry streaming with controllable noise and fault injection.

### FR-02: Feature Engineering & Preprocessing
- **FR-02.1**: The preprocessor shall serialize telemetry into a standard 18-dimensional numerical feature vector.
- **FR-02.2**: The preprocessor shall maintain a rolling historical window and calculate the first temporal derivative of SoC temperature ($\Delta T / \Delta t$) in ${^\circ}\text{C}/\text{sec}$.
- **FR-02.3**: The preprocessor shall compute the first temporal derivative of interrupt arrival rates ($\Delta \text{IRQ} / \Delta t$) in $\text{IRQs}/\text{sec}^2$.

### FR-03: Machine Learning Anomaly Detection
- **FR-03.1**: The inference engine shall employ an unsupervised Isolation Forest model trained on nominal operational telemetry.
- **FR-03.2**: The model shall evaluate feature vectors and generate a raw decision function score in $[-0.5, 0.5]$.
- **FR-03.3**: The engine shall project raw decision scores to a calibrated 0.0% to 100.0% Risk Score using a continuous sigmoid mapping function.
- **FR-03.4**: The anomaly detector shall maintain consecutive anomaly counters to distinguish transient telemetry spikes from sustained hardware degradation.

### FR-04: Root-Cause Classification & Diagnostics
- **FR-04.1**: The system shall evaluate feature signatures to classify root causes: Thermal Runaway, Interrupt Storm, Memory Exhaustion & Swap Thrashing, I/O Wait Bottleneck, and Network Drop Surge.
- **FR-04.2**: The diagnostic engine shall produce human-readable diagnostic summaries and specific recommended mitigation steps for system operators.

### FR-05: Real-Time Visualization & Dashboard
- **FR-05.1**: The service shall expose REST API endpoints (`/api/telemetry`, `/api/inject_anomaly`, `/api/history`) over HTTP.
- **FR-05.2**: The service shall render a responsive glassmorphism web dashboard featuring real-time Chart.js graphs for SoC temperature, CPU utilization, kernel IRQs, and Risk Score.
- **FR-05.3**: The dashboard shall provide interactive one-click buttons to inject and clear synthetic fault conditions.

---

## 3. Non-Functional Requirements

- **NFR-01 (Low Overhead)**: The telemetry collection and inference pipeline shall consume less than 1.5% CPU and less than 35 MB of resident RAM on standard ARM/x86 Linux hardware.
- **NFR-02 (Low Latency)**: Inference cycle time per sample shall not exceed 10 milliseconds.
- **NFR-03 (Fault Tolerance)**: Telemetry parsing shall gracefully catch and recover from missing virtual files or unreadable sysfs nodes without crashing the daemon.
- **NFR-04 (Cross-Platform Execution)**: The software shall support dual-mode execution (native Linux procfs/sysfs and simulated synthetic mode on developer workstations).
