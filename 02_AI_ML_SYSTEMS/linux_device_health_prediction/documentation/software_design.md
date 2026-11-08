# Linux Device Health Prediction AI - Software Design Document

## 1. Modular Architecture

The software architecture follows a decoupled, pipe-and-filter design where raw OS metrics undergo progressive transformation, enrichment, statistical inference, and diagnostic classification.

```
[Linux Virtual FS] ---> [Kernel Parsers] ---> [Data Collector] ---> [Feature Preprocessor]
                                                                            |
                                                                            v
[Dashboard UI] <--- [Flask REST Server] <--- [Risk Classifier] <--- [Isolation Forest]
```

### Module Responsibilities:
1. `kernel_metrics/`:
   - `procfs_parser.py`: Low-overhead string parsers for `/proc/stat`, `/proc/loadavg`, and `/proc/meminfo`.
   - `sysfs_parser.py`: Hardware bus and thermal directory tree scanners with millidegree unit conversions.
2. `telemetry/`:
   - `schema.py`: Type-safe dataclass definitions for structured packets, flat vector serialization, and feature name mappings.
3. `data_collection/`:
   - `linux_collector.py`: Environment-aware collector switching between native procfs/sysfs and synthetic anomaly streaming.
4. `feature_engineering/`:
   - `preprocessor.py`: Rolling window queue managing delta time $\Delta t$ and temporal feature derivatives.
5. `model_training/`:
   - `train.py`: Generates 1500 baseline samples, fits `IsolationForest(n_estimators=100, contamination=0.03, n_jobs=1)`, validates synthetic faults, and outputs `model.joblib`.
6. `inference_engine/`:
   - `predictor.py`: Loads serialized model, computes decision score, and maps to calibrated continuous risk percentages.
7. `anomaly_detection/`:
   - `detector.py`: Stateful hysteresis filter smoothing out single-tick transient spikes using consecutive anomaly counters.
8. `risk_assessment/`:
   - `risk_classifier.py`: Rule-assisted root-cause diagnosis generating human-readable alerts and mitigation actions.
9. `linux_agent/`:
   - `daemon.py`: Background agent CLI orchestrating cycle loops, console outputs, and headless background execution.
10. `dashboard/`:
    - `server.py`: Flask HTTP server serving `/` and JSON API endpoints.
    - `templates/index.html`: Responsive dark-mode glassmorphism interface powered by Chart.js.

---

## 2. Interface Contracts & Data Formats

### 2.1 Flat Feature Vector Structure
- Dimension: $18 \times 1$ float32 vector.
- Vector ordering guarantees strict alignment between `train.py`, `predictor.py`, and `detector.py`.

### 2.2 REST API Specification

#### `GET /api/telemetry`
Returns the latest evaluated health snapshot:
```json
{
  "timestamp": 1773045600.5,
  "hostname": "edge-gateway-01",
  "telemetry": {
    "cpu": { "user_pct": 14.2, "system_pct": 5.1, "iowait_pct": 0.3, "loadavg_1m": 0.85 },
    "memory": { "used_pct": 42.1, "swap_used_pct": 0.0, "available_mb": 4742.0 },
    "thermal": { "soc_max_temp_c": 44.2, "throttled_state": 0 }
  },
  "detection": {
    "prediction": 1,
    "risk_score_pct": 21.4,
    "decision_score": 0.1142,
    "classification": "HEALTHY"
  },
  "diagnosis": {
    "primary_diagnosis": "Nominal operating health across all monitored Linux subsystems.",
    "culprits": [],
    "recommended_mitigations": []
  }
}
```

#### `POST /api/inject_anomaly`
Payload:
```json
{ "anomaly_type": "interrupt_storm" }
```
Response:
```json
{ "status": "injected", "anomaly_type": "interrupt_storm" }
```
