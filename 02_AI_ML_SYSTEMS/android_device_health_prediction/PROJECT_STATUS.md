# Project Status: Android Device Health Prediction AI

- **Project Name:** `android_device_health_prediction`
- **Category:** `02_AI_ML_SYSTEMS`
- **Development Status:** `Testing / Validated`
- **Current Version:** `v1.2.0`
- **Lead Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))

---

## 1. Specifications & Technologies
- **Telemetry Ingestion:** Android Debug Bridge (ADB shell dumpsys) + Synthetic Simulation Engine
- **Machine Learning Core:** Isolation Forest (100 Decision Trees, Unsupervised, scikit-learn)
- **Feature Pipeline:** 14-dimensional feature vector with thermal gradients and memory leak velocity
- **Risk Assessment:** Multi-tier calibrated scoring (0 - 100%) with root-cause diagnostic attribution
- **Visualization:** Glassmorphism Interactive Web Dashboard (Flask + Chart.js)

---

## 2. Verification Summary
- **Model Training Pipeline:** Verified; generates 1,200 sample baseline and saves artifacts.
- **Inference Engine:** Verified; evaluates single-vector inference in 6.8 ms.
- **Anomaly Detection Accuracy:** Verified across simulated thermal runaway, memory leaks, and CPU hogs.
- **Automated Tests:** 100% passed in `tests/test_android_ai.py`.
