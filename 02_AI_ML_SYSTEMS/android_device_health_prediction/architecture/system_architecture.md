# System Architecture: Android Predictive Device Health AI

**Domain:** Edge AI, Android OS Telemetry, Autonomous Failure Diagnostics  
**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  

---

## 1. Architectural Overview

The **Android Device Health Prediction AI System** monitors operating system dynamics, silicon thermal envelopes, and peripheral subsystems. By coupling low-overhead ADB telemetry collection with an unsupervised Isolation Forest model, the platform detects hardware and software anomalies before fatal kernel panics, thermal shutdowns, or Out-Of-Memory (OOM) app crashes:

```mermaid
flowchart TD
    subgraph Android_Target ["Android Device Layer (AOSP / Physical / Kiosk)"]
        DumpsysBattery["dumpsys battery (Thermal, Level)"]
        DumpsysMem["dumpsys meminfo (Used, Free, LMK)"]
        DumpsysCPU["dumpsys cpuinfo (User, Kernel, Top Procs)"]
        DumpsysMedia["dumpsys media.camera / bluetooth_manager"]
    end

    subgraph Data_Acquisition ["Data Collection & Ingestion Layer"]
        ADBBridging["ADB Bridge / ADBDeviceCollector"]
        SyntheticSim["Synthetic Telemetry Simulator (Fallback)"]
        RawStream["Structured AndroidTelemetryPacket"]
    end

    subgraph Feature_Engineering ["Feature Engineering & Derivation Layer"]
        RollingWindow["Rolling Window Deque (N=5 Frames)"]
        TemporalDerivs["Temporal Gradient Calculators (dT/dt, dRAM/dt)"]
        EnrichedVector["14-Dimensional Feature Vector"]
    end

    subgraph AI_Inference ["Unsupervised ML & Risk Assessment Engine"]
        IsoForest["Isolation Forest (100 Decision Trees)"]
        ScoreMapper["Decision Function to Risk % Normalizer"]
        RootCauseEngine["Multi-Domain Diagnostic Assessor"]
    end

    subgraph Presentation ["Observability & Remediation Layer"]
        AgentCLI["Application Health Agent Daemon"]
        REST_API["Flask REST API Service (:5050)"]
        GlassDashboard["Glassmorphism Real-Time Web Dashboard"]
    end

    Android_Target --> ADBBridging
    ADBBridging --> RawStream
    SyntheticSim -.-> RawStream
    RawStream --> RollingWindow
    RollingWindow --> TemporalDerivs
    TemporalDerivs --> EnrichedVector

    EnrichedVector --> IsoForest
    IsoForest --> ScoreMapper
    ScoreMapper --> RootCauseEngine

    RootCauseEngine --> AgentCLI
    AgentCLI --> REST_API
    REST_API --> GlassDashboard
```
