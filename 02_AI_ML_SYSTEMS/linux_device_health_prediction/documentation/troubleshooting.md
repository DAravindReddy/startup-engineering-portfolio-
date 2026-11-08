# Linux Device Health Prediction AI - Troubleshooting Guide

## 1. Diagnostic Fault Matrix

| Symptom / Observation | Potential Cause | Verification Step | Remediation |
|:---|:---|:---|:---|
| `[LinuxHealthPredictor] Model file not found` | The model artifact `model.joblib` has not been generated or deployed. | Check if `inference_engine/model.joblib` exists on disk. | Execute `python model_training/train.py` to generate and deploy model artifacts. |
| Dashboard shows static or non-updating charts | Browser WebSocket/HTTP polling error or Flask server unreachable. | Inspect browser DevTools Console (`F12`) for HTTP 500 or CORS errors. | Verify Flask is running on port 8082 with `curl http://localhost:8082/api/telemetry`. |
| Missing procfs permissions on restricted Linux hosts | Non-root users may have restricted access to certain `/proc` or `/sys` nodes on hardened systems. | Run `cat /proc/stat` and `cat /sys/class/thermal/thermal_zone0/temp`. | Add monitoring user to appropriate group or grant read permissions via udev rules. |
| Persistent `CRITICAL_ANOMALY` in normal operation | Model contamination hyperparameter mismatch or baseline drift caused by new hardware. | Check decision score in `/api/telemetry`. | Retrain model on target hardware using `python model_training/train.py`. |
| High CPU overhead from monitoring daemon | Polling interval is set too aggressively (< 0.2 seconds). | Check `top` for `python` process utilization. | Increase polling interval with `--interval 2.0` or `--interval 5.0`. |

---

## 2. Emergency Recovery Playbook

If a critical failure condition is detected on an edge device:

### Scenario A: Thermal Runaway ($T > 85^\circ\text{C}$)
```bash
# Query active thermal governor
cat /sys/class/thermal/thermal_zone0/policy
# Force conservative governor
echo "powersave" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
```

### Scenario B: Interrupt Storm (> 50,000 IRQs/sec)
```bash
# Identify high-frequency interrupt lines
cat /proc/interrupts | sort -k2 -n -r | head -n 15
# Inspect device driver associated with culprit IRQ line
lspci -k | grep -E "(Kernel driver|Ethernet|USB)"
```

### Scenario C: Out of Memory / Swap Thrashing
```bash
# Identify top memory consumers
ps aux --sort=-%mem | head -n 10
# Clear disk caches if memory fragmentation is high
sync; echo 3 | sudo tee /proc/sys/vm/drop_caches
```
