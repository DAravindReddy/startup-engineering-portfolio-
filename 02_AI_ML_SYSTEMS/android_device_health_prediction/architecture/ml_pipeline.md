# Machine Learning Pipeline Specification: Isolation Forest Anomaly Detection

**Project:** Android Device Health Prediction AI  

---

## 1. Machine Learning Pipeline Architecture

```mermaid
graph LR
    subgraph Data_Preparation ["1. Data Acquisition & Preprocessing"]
        A1["Baseline Healthy Android Telemetry<br/>(Idle, Web, Audio, Gaming)"]
        A2["Telemetry Feature Schema<br/>(CPU, GPU, Thermal, RAM, Subsystems)"]
        A3["Temporal Rolling Derivations<br/>(dT/dt, dRAM/dt)"]
        A1 --> A2 --> A3
    end

    subgraph Offline_Training ["2. Model Training & Serialization"]
        B1["Feature Matrix X (1200 Samples, 14 Features)"]
        B2["Isolation Forest Ensemble<br/>(n_estimators=100, contamination=0.04)"]
        B3["Artifact Packaging<br/>(model.joblib + metadata)"]
        A3 --> B1 --> B2 --> B3
    end

    subgraph Online_Inference ["3. Real-Time Inference & Risk Assessment"]
        C1["Incoming Streaming Feature Vector"]
        C2["Decision Function Evaluation<br/>(Mean Path Length Calculation)"]
        C3["Calibrated Risk Score Mapping (0 - 100%)"]
        C4["Categorical Status & Diagnostic Attribution<br/>(HEALTHY / ELEVATED / CRITICAL)"]
        B3 -.-> C2
        C1 --> C2 --> C3 --> C4
    end
```

---

## 2. Mathematical Formulation: Isolation Forest

The fundamental premise of Isolation Forests is that anomalies are 'few and different', meaning they require significantly fewer random splits to isolate within a decision tree than normal instances:

### A. Average Path Length Formulation
The average path length $c(n)$ of unsuccessful searches in a Binary Search Tree (BST) of $n$ instances is given by:

$$c(n) = 2 \ln(n - 1) + 0.5772156649 \text{ (Euler-Mascheroni Constant)} - \frac{2(n - 1)}{n}$$

### B. Anomaly Score $s(x, n)$
For an input feature vector $x$ across an ensemble of $T$ trees, each yielding path length $h_i(x)$:

$$E(h(x)) = \frac{1}{T} \sum_{i=1}^{T} h_i(x)$$

The anomaly score $s \in [0, 1]$ is:

$$s(x, n) = 2^{-\frac{E(h(x))}{c(n)}}$$

- When $E(h(x)) \to 0 \implies s \to 1$: Sample is isolated immediately near the tree root $\implies$ **Definite Anomaly**.
- When $E(h(x)) \to c(n) \implies s \to 0.5$: Sample behaves like typical instances $\implies$ **Normal Baseline**.
- When $E(h(x)) \to n - 1 \implies s \to 0$: Sample requires deep recursive partitioning $\implies$ **Safe Cluster**.
