# Installation and Execution Guide: Android Health AI

**Project:** Android Device Health Prediction AI  

---

## 1. Prerequisites
- **Python Runtime:** Python 3.10+ (Python 3.12 verified).
- **Android Platform Tools (Optional for physical hardware):**
  - Install `adb` CLI from Android SDK Platform Tools.
  - Enable **Developer Options** and **USB Debugging** on the target Android device.
  - Ensure device is listed when running `adb devices`.
  *(Note: If ADB is absent or no device is attached, the system automatically runs the synthetic telemetry emulator!)*

---

## 2. Environment Setup

```bash
cd 02_AI_ML_SYSTEMS/android_device_health_prediction

# Create Virtual Environment (Optional but recommended)
python -m venv venv
# Windows:
.\venv\Scripts\activate
# Linux/macOS:
source venv/bin/activate

# Install dependencies
pip install -r requirements.txt
```

---

## 3. Running Model Training

To retrain the Isolation Forest model on a fresh synthetic baseline:

```bash
python model_training/isolation_forest/train.py
```

*Output:*
```text
[*] Generating 1200 normal Android telemetry training vectors...
[*] Training Isolation Forest (n_estimators=100, contamination=0.04)...
[+] Model training complete!
[+] Serialized model artifact saved to: model_training/isolation_forest/model.joblib
[+] Synced model artifact to inference engine: inference_engine/model.joblib
```

---

## 4. Running the Monitoring Agent Daemon

### Quick 5-Step Verification Demo:
```bash
python application/health_agent.py --demo
```

### Continuous Background Monitoring with Anomaly Testing:
```bash
# Normal continuous monitoring
python application/health_agent.py --interval 2.0

# Simulate specific hardware failure:
python application/health_agent.py --anomaly thermal
python application/health_agent.py --anomaly memory_leak
python application/health_agent.py --anomaly cpu_hog
```

---

## 5. Launching the Interactive Web Dashboard

```bash
python dashboard/server.py
```
Open your web browser and navigate to:
**`http://localhost:5050/`**

---

## 6. Running the Automated Test Suite

```bash
python tests/test_android_ai.py
```
