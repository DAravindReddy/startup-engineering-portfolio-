# Proposed Architectural Solution & Contributions

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Architectural Innovations

To eliminate the failure modes identified in the problem statement, this project implements a tripartite engineering framework:

1. **Non-Blocking Cooperative Time-Sliced Scheduler:**
   - Every peripheral task (Bluetooth RX ingestion, soil moisture sampling, DHT11 acquisition, telemetry serial broadcasting, and heartbeat flashing) is decoupled into dedicated non-blocking time slots evaluated against `millis()` delta checks.
   - Loop execution frequencies exceed 10 kHz, guaranteeing near-instantaneous Bluetooth command handling (<10 ms).
2. **Dual-Threshold Hysteretic Finite State Machine (FSM):**
   - The irrigation controller introduces a deadband hysteresis gap of 30% between dry activation ($\theta_{\text{dry}} = 35\%$) and wet deactivation ($\theta_{\text{wet}} = 65\%$).
   - A mandatory minimum rest cooldown period ($\tau_{\text{cooldown}} = 5000\text{ ms}$) is enforced between pumping cycles to ensure accurate hydraulic percolation before re-sampling.
3. **Hardware Safety Watchdog Supervisor:**
   - A hard safety limit ($\tau_{\text{max\_run}} = 15000\text{ ms}$) is continuously evaluated inside `irrigation_update()`.
   - If pump actuation exceeds this duration, the system unconditionally de-energizes the relay and trips into `PUMP_STATE_SAFETY_TRIP`, requiring an explicit remote or physical reset.
