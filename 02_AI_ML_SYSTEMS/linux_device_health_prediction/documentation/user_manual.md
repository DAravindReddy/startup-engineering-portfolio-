# Linux Device Health Prediction AI - User Manual

## 1. Operating the Command Line Agent

The `linux_agent/daemon.py` utility is the primary entry point for headless execution, automated scripts, and systemd service integration.

### Command Line Options:
```bash
python linux_agent/daemon.py [OPTIONS]
```

- `--synthetic`: Forces synthetic telemetry streaming (useful on developer laptops without Linux virtual filesystems).
- `--interval <FLOAT>`: Polling cycle duration in seconds (default: `2.0`).
- `--iterations <INT>`: Number of sampling loops before terminating (`0` runs indefinitely).
- `--anomaly <NAME>`: Injects a specific continuous fault condition. Supported modes:
  - `thermal_throttling`: SoC runaway temperature (>88°C) and governor throttling.
  - `memory_exhaustion`: Saturated RAM (>96%) and swap thrashing.
  - `interrupt_storm`: Spurious IRQ storm (>70,000 IRQs/sec) and high context switching.
  - `iowait_thrashing`: Blocked tasks in D-state (>15) and saturated disk I/O.
  - `network_packet_loss`: High ring buffer packet drops (>1,500 drops/sec).
- `--demo`: Executes an automated 5-step test sequence verifying baseline, fault injection, and recovery.

---

## 2. Using the Web Dashboard

The web dashboard provides an executive glassmorphism visualization interface:

### 2.1 KPI Cards
- **Health Risk Score**: Displays real-time risk percentage (0.0% to 100.0%) and status pill (`HEALTHY`, `ELEVATED_RISK`, `CRITICAL_ANOMALY`).
- **CPU Load & IO Wait**: Aggregated user and system CPU percentage, 1-minute load average, and IO wait time.
- **Memory & Swap**: RAM utilization, free memory, and swap space consumption.
- **SoC Thermal Zone**: Peak core junction temperature and active CPU governor throttling state.
- **Kernel Interrupts**: Real-time interrupt rate per second and context switch frequency.

### 2.2 Interactive Fault Injection
Click any of the red fault buttons in the sidebar to simulate critical failure modes:
1. Click **Interrupt Storm** $\rightarrow$ Observe IRQs spike to ~85,000/s, Risk Score rise to >65%, and status transition to `CRITICAL_ANOMALY`.
2. Inspect the **Root-Cause Diagnostic Engine** box $\rightarrow$ See exact culprit identification and recommended mitigations.
3. Click **Reset to Nominal Stream** $\rightarrow$ Watch the system recover to nominal baseline.

---

## 3. Integrating with Systemd as an Edge Service

To run the agent as a background daemon on a production Linux host:

Create `/etc/systemd/system/linux-health-agent.service`:
```ini
[Unit]
Description=Linux Device Health AI Monitoring Service
After=network.target

[Service]
Type=simple
User=root
WorkingDirectory=/opt/startup-engineering-portfolio/02_AI_ML_SYSTEMS/linux_device_health_prediction
ExecStart=/usr/bin/python3 linux_agent/daemon.py --interval 2.0
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
```

Enable and start the service:
```bash
sudo systemctl daemon-reload
sudo systemctl enable --now linux-health-agent.service
sudo systemctl status linux-health-agent.service
```
