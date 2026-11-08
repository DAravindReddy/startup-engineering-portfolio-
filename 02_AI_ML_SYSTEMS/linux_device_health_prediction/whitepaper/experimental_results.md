# Experimental Results & Performance Benchmarks

## 1. Anomaly Detection Validation
The trained Isolation Forest model was evaluated across 5 controlled synthetic failure scenarios.

| Scenario | Injected Condition | Observed Score | Calibrated Risk (%) | Detection Status | Latency |
|:---|:---|:---:|:---:|:---:|:---:|
| **Baseline** | Normal userspace activity | $+0.1245$ | $21.6\%$ | `HEALTHY` | N/A |
| **Interrupt Storm** | 85,356 IRQs/s | $-0.0821$ | $69.4\%$ | `CRITICAL_ANOMALY` | < 0.5s |
| **Thermal Throttling** | $89.5^\circ\text{C}$, State 3 | $-0.0582$ | $74.1\%$ | `CRITICAL_ANOMALY` | < 1.0s |
| **Memory Exhaustion** | 97.2% RAM, 85% Swap | $-0.0489$ | $68.2\%$ | `CRITICAL_ANOMALY` | < 1.0s |
| **I/O Wait Thrashing** | 72% IO-wait, 18 procs | $-0.0614$ | $72.3\%$ | `CRITICAL_ANOMALY` | < 1.0s |
| **Network Loss Surge** | 2,400 drops/s | $-0.0340$ | $64.8\%$ | `CRITICAL_ANOMALY` | < 2.0s |

## 2. Resource Utilization on Edge Hardware
- **Inference Latency:** Mean $1.42\text{ ms}$ per sample.
- **CPU Utilization:** $< 0.8\%$ of a single core at $2.0\text{ s}$ sampling period.
- **Resident Memory (RSS):** $28.4\text{ MB}$.
- **Zero Third-Party OS Daemons:** No background Prometheus or Node Exporters required.
