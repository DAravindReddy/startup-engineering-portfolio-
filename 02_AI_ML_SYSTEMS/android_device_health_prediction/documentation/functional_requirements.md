# Functional Requirements Traceability Matrix: Android Health AI

**Project:** Android Device Health Prediction AI  

---

| Req ID | Requirement Description | Implementation Module | Verification Method | Status |
|:---:|---|---|---|:---:|
| **AHP-01** | Real-time ADB telemetry collection from physical Android devices via dumpsys. | `adb_collector.py` | Subprocess execution & live regex parse test. | **VERIFIED** |
| **AHP-02** | Synthetic telemetry emulator fallback with configurable anomaly injection. | `adb_collector.py` | Automated synthetic stream generation. | **VERIFIED** |
| **AHP-03** | Multi-domain monitoring (CPU, GPU, thermal, memory, Bluetooth, camera, media). | `schema.py` | Data schema validation tests. | **VERIFIED** |
| **AHP-04** | Time-series feature engineering with thermal gradient and memory leak velocity. | `preprocessor.py` | Pytest mathematical gradient assertions. | **VERIFIED** |
| **AHP-05** | Unsupervised Isolation Forest model training on baseline operating telemetry. | `train.py` | Model training pipeline & joblib serialization. | **VERIFIED** |
| **AHP-06** | Real-time on-device inference with calibrated risk score % mapping. | `predictor.py` | Decision score to risk % validation. | **VERIFIED** |
| **AHP-07** | Automated root-cause diagnostic attribution and action recommendation. | `risk_classifier.py` | Thermal, memory, and CPU anomaly tests. | **VERIFIED** |
| **AHP-08** | Continuous background health monitoring agent daemon with CLI flags. | `health_agent.py` | 5-step automated demo run (`--demo`). | **VERIFIED** |
| **AHP-09** | Interactive glassmorphism web dashboard with live Chart.js visualizations. | `server.py`, `index.html` | Flask REST endpoint & UI verification. | **VERIFIED** |
| **AHP-10** | Interactive simulation controls for testing thermal, memory, and CPU anomalies. | `server.py`, `index.html` | `/api/set_anomaly` POST trigger test. | **VERIFIED** |
