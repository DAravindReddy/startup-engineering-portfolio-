# Linux Device Health Prediction AI - System Architecture

## 1. Architectural Overview

The Linux Device Health Prediction AI architecture is an autonomous edge intelligence service designed to detect subtle, multi-variable operating system anomalies, kernel lockups, and hardware degradation prior to catastrophic system crashes or thermal panics.

The system decouples operating system metric extraction, continuous signal conditioning, unsupervised anomaly scoring, and rule-assisted root-cause diagnosis.

```mermaid
flowchart TD
    subgraph LinuxKernel ["Linux Kernel & Hardware Space"]
        ProcFS["/proc Virtual Filesystem\n(/proc/stat, /proc/loadavg, /proc/meminfo)"]
        SysFS["/sys Virtual Filesystem\n(/sys/class/thermal, /sys/bus/usb)"]
        HW["Hardware Sensors & Peripherals\n(SoC Temp, NIC Drops, UART Overrun)"]
    end

    subgraph TelemetryLayer ["Telemetry Ingestion Layer"]
        ProcParser["ProcfsParser\n(CPU, RAM, ctxt, procs)"]
        SysParser["SysfsParser\n(Thermal Zones, Cdevs, Buses)"]
        PacketBuilder["LinuxTelemetryPacket\n(16-D Base Serialization)"]
    end

    subgraph FeaturePipeline ["Feature Engineering Engine"]
        RollingBuf["Temporal Rolling Buffer\n(History Window: N=5)"]
        DerivCalc["Derivative Computation\n(ΔT_SoC / Δt, ΔIRQ / Δt)"]
        VectorOut["18-D Engineered Feature Vector"]
    end

    subgraph MLInference ["Unsupervised Anomaly Detection"]
        IsoForest["Isolation Forest Model\n(100 iTrees, Contamination 3%)"]
        ScoreMap["Continuous Sigmoidal Risk Projection\n(Decision Score -> 0.0% - 100.0%)"]
        StateGate["Consecutive Anomaly Filter\n(HEALTHY / ELEVATED / CRITICAL)"]
    end

    subgraph RootCauseDiagnosis ["Diagnostic & Action Layer"]
        RuleClassifier["Multi-Subsystem Root-Cause Classifier"]
        Remediation["Admin Mitigation Actions\n(cgroup limits, irq affinity, governor)"]
        WebDash["Glassmorphism Real-Time Web Dashboard\n(Chart.js & Fault Injector)"]
    end

    ProcFS --> ProcParser
    SysFS --> SysParser
    HW --> SysParser
    ProcParser --> PacketBuilder
    SysParser --> PacketBuilder

    PacketBuilder --> RollingBuf
    RollingBuf --> DerivCalc
    DerivCalc --> VectorOut

    VectorOut --> IsoForest
    IsoForest --> ScoreMap
    ScoreMap --> StateGate

    StateGate --> RuleClassifier
    PacketBuilder --> RuleClassifier
    RuleClassifier --> Remediation
    RuleClassifier --> WebDash
```

## 2. Ingestion & Preprocessing Subsystems

### 2.1 Virtual Filesystem Sampling
1. **/proc/stat & /proc/loadavg**: Computes user vs. system vs. iowait percentages and context switches per second without incurring heavy userspace overhead.
2. **/proc/meminfo**: Computes resident vs. cached memory, dirty buffers, and swap thrashing rate.
3. **/sys/class/thermal**: Collects physical SoC junction temperatures across discrete thermal zones and records active cooling device governor states.

### 2.2 Feature Vector Mathematical Representation

The 18-dimensional feature vector $\mathbf{x}_t \in \mathbb{R}^{18}$ at timestamp $t$ is formulated as:

$$\mathbf{x}_t = \begin{bmatrix}
\%CPU_{\text{user}}, & \%CPU_{\text{sys}}, & \%CPU_{\text{iowait}}, & Load_{\text{1m}}, & CSW/s, \\
\%RAM_{\text{used}}, & \%Swap_{\text{used}}, & T_{\text{SoC}}, & State_{\text{throttle}}, & IRQ/s, \\
Proc_{\text{run}}, & Proc_{\text{block}}, & Proc_{\text{zombie}}, & Rate_{\text{disk\_w}}, & Drop_{\text{net}}, \\
Err_{\text{uart}}, & \frac{\Delta T_{\text{SoC}}}{\Delta t}, & \frac{\Delta IRQ}{\Delta t}
\end{bmatrix}^T$$

Where:
- $\frac{\Delta T_{\text{SoC}}}{\Delta t} = \frac{T_t - T_{t-1}}{t - t-1}$ represents rate of thermal rise in ${^\circ}\text{C}/\text{sec}$.
- $\frac{\Delta IRQ}{\Delta t} = \frac{IRQ_t - IRQ_{t-1}}{t - t-1}$ represents the acceleration of hardware interrupts in $\text{IRQs}/\text{sec}^2$.
