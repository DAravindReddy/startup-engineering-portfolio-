# Arduino Nano 33 BLE Sense AI - System Architecture

## 1. Architectural Overview

The Arduino Nano 33 BLE Sense Machine Health AI is an autonomous, on-device TinyML predictive maintenance node. It executes real-time sensor sampling, rolling-window statistical feature engineering, on-device anomaly scoring, and Bluetooth Low Energy (BLE) GATT telemetry streaming directly on the ARM Cortex-M4F microcontroller.

```mermaid
flowchart TD
    subgraph OnboardSensors ["Onboard Physical Sensor Space"]
        LSM["LSM9DS1 3-Axis Accelerometer\n(100 Hz Sampling Rate)"]
        HTS["HTS221 Thermal & Humidity Sensor\n(1 Hz Sampling Rate)"]
    end

    subgraph FeatureEngineering ["On-Device Signal Conditioning"]
        RingBuf["Rolling Window Ring Buffer\n(N=64 Acceleration Samples)"]
        StatCalc["Statistical Feature Extractor\n(RMS, Pk-Pk, Crest Factor, ΔT/Δt)"]
        FeatureVec["5-D Extracted Feature Vector"]
    end

    subgraph TinyMLInference ["TinyML Anomaly Inference Engine"]
        ZNorm["Standardized Z-Score Normalization\n(Z_rms, Z_pkpk, Z_temp)"]
        DistEngine["Mahalanobis Anomaly Distance"]
        SigmoidMap["Calibrated Continuous Risk Projection\n(0.0% to 100.0% Anomaly Index)"]
        StateGate["Classifier (NORMAL / WARNING / CRITICAL)"]
    end

    subgraph BLESvc ["BLE 5.0 GATT Telemetry Server"]
        Service["Machine Health Service (UUID: 19B10000...)"]
        CharScore["Char: Anomaly Score (%)"]
        CharRMS["Char: Vibration RMS (m/s²)"]
        CharTemp["Char: Temperature (°C)"]
        CharState["Char: Machine Operational State"]
        Client["Connected BLE Central\n(Smartphone / Edge Gateway)"]
    end

    LSM --> RingBuf
    RingBuf --> StatCalc
    HTS --> StatCalc
    StatCalc --> FeatureVec

    FeatureVec --> ZNorm
    ZNorm --> DistEngine
    DistEngine --> SigmoidMap
    SigmoidMap --> StateGate

    StateGate --> Service
    Service --> CharScore
    Service --> CharRMS
    Service --> CharTemp
    Service --> CharState
    Service -->|Wireless Notifications| Client
```

---

## 2. Mathematical Pipeline Formulation

$$\text{RMS}_{\text{vibe}} = \sqrt{\frac{1}{N} \sum_{i=1}^N \left(a_i - \bar{a}\right)^2}$$

$$d^2 = 1.5 \left(\frac{\text{RMS} - \mu_{\text{RMS}}}{\sigma_{\text{RMS}}}\right)^2 + 0.8 \left(\frac{A_{\text{pk}} - \mu_{\text{pk}}}{\sigma_{\text{pk}}}\right)^2 + 1.2 \left(\frac{T - \mu_T}{\sigma_T}\right)^2$$

$$\text{Score}_{\text{anomaly}} = \frac{100.0}{1 + e^{-0.45(d^2 - 6.0)}}\quad [\%]$$
