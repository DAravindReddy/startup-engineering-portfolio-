# Problem Statement: Operating System Failures in Mission-Critical Edge Deployments

## 1. Context and Motivation
Edge Linux nodes deployed in robotics, automotive telematics, energy monitoring, and remote IoT gateways are required to maintain continuous availability under harsh operational conditions. Unlike cloud servers in climate-controlled data centers, edge nodes face severe challenges:
- High thermal cycling and ambient temperature swings.
- Unstable peripheral power rails and EMI line ringing.
- Unbounded process memory allocation on constrained hardware (1 GB - 8 GB RAM).

## 2. Failure Modes in Edge Computing
1. **Thermal Choking & Runaway:** Passive heatsink detachment or dust accumulation causes rapid temperature spikes. When the junction temperature exceeds $85^\circ\text{C}$, the kernel throttles frequencies, triggering deadlines missed in real-time control loops.
2. **Spurious Interrupt Storms:** External sensor lines or faulty transceiver silicon enter high-frequency floating states, generating over 80,000 hardware interrupts per second. The kernel spends 90%+ of CPU cycles in the IRQ handler, starving user processes.
3. **Memory Thrashing:** Memory leaks force swap file saturation, driving disk I/O wait times to 80% and locking up filesystem operations.

## 3. The Failure of Static Thresholds
Traditional monitoring solutions utilize fixed threshold rules (e.g. CPU > 85%, RAM > 90%). These rules fail because:
- Normal burst tasks (e.g. software updates or batch processing) trigger false alarms.
- Multi-dimensional subtle degradation (e.g., moderate CPU + rising thermal acceleration + increasing interrupt rate) goes undetected until after the kernel crashes.
