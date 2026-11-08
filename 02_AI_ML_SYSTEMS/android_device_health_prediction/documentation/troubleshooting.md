# Troubleshooting Guide: Android Health AI

**Project:** Android Device Health Prediction AI  

---

## 1. Diagnostic Matrix

| Problem / Symptom | Probable Cause | Remediation Procedure |
|---|---|---|
| **Collector falls back to MOCK mode despite phone plugged in** | ADB daemon not running, device unauthorized, or USB Debugging disabled. | Run `adb devices` in terminal. Check phone screen for "Allow USB Debugging" prompt. Verify USB cable supports data transfer. |
| **`FileNotFoundError: model.joblib not found`** | Model training pipeline was not executed before running inference. | Execute `python model_training/isolation_forest/train.py` to compile baseline weights and feature metadata. |
| **Flask dashboard port 5050 already in use** | Another instance of `server.py` or local service is bound to port 5050. | Modify port in `dashboard/server.py` line 48 or terminate conflicting process with `Stop-Process -Id (Get-NetTCPConnection -LocalPort 5050).OwningProcess`. |
| **High CPU usage during Python telemetry execution** | Too rapid polling interval. | Set `--interval 2.0` or higher to minimize ADB shell subprocess overhead. |
