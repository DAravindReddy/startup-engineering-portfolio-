# Linux Device Health Prediction AI - Installation Guide

## 1. Prerequisites

### Supported Platforms
- **Embedded Linux**: Raspberry Pi OS, Yocto Project Linux, Buildroot, Ubuntu Core, Debian, Armbian.
- **Enterprise Linux**: Ubuntu 20.04/22.04/24.04 LTS, RHEL/Rocky Linux 8/9.
- **Development Workstations**: Windows 10/11, macOS (runs in synthetic simulation mode).

### Software Requirements
- Python 3.10+ (tested on Python 3.12).
- Standard build utilities (`gcc`, `make` optional for native extensions).
- Modern web browser for dashboard visualization (Chrome, Firefox, Edge, Safari).

---

## 2. Installation Steps

### 2.1 Clone / Access Directory
```bash
cd 02_AI_ML_SYSTEMS/linux_device_health_prediction
```

### 2.2 Install Dependencies
Install required packages via pip:
```bash
pip install -r requirements.txt
```

Package contents (`requirements.txt`):
```text
numpy>=1.24.0
scikit-learn>=1.3.0
scipy>=1.10.0
joblib>=1.3.0
Flask>=3.0.0
```

---

## 3. Model Training & Verification

### 3.1 Train Baseline Model
Train the Isolation Forest model on nominal operating baseline data:
```bash
python model_training/train.py
```
Expected output:
```text
Generating 1500 baseline nominal telemetry vectors...
Dataset shape: (1500, 18) (18 dimensions)
Fitting Isolation Forest model (n_estimators=100, contamination=0.03, n_jobs=1)...
Nominal training outlier rate: 3.00% (Nominal threshold verified)
Model saved to: model_training/model.joblib
Model deployed to: inference_engine/model.joblib
```

### 3.2 Run Automated Test Suite
Validate all parsers, feature preprocessors, ML predictors, and risk classifiers:
```bash
python -m unittest tests/test_linux_ai.py
```
Output:
```text
Ran 7 tests in 1.2s
OK
```

---

## 4. Running the Application

### 4.1 Automated CLI Demo
Run an end-to-end 5-step demonstration with fault injection:
```bash
python linux_agent/daemon.py --demo
```

### 4.2 Start Live Health Monitoring Agent
Run the autonomous background monitoring loop:
```bash
python linux_agent/daemon.py --interval 2.0
```

### 4.3 Launch Real-Time Glassmorphism Web Dashboard
Start the web dashboard on port 8082:
```bash
python dashboard/server.py
```
Navigate your browser to:
```
http://localhost:8082
```
