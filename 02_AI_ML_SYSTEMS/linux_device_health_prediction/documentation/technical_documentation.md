# Linux Device Health Prediction AI - Technical Documentation

## 1. Executive Summary

The **Linux Device Health Prediction AI** is an intelligent, autonomous operating system health monitoring engine engineered for embedded Linux platforms, edge gateways, industrial controllers, and enterprise servers. By combining non-intrusive procfs/sysfs kernel telemetry parsing with an unsupervised 18-dimensional Isolation Forest machine learning model, the system identifies anomalous system degradation—such as thermal runaway, spurious interrupt storms, swap thrashing, and peripheral driver stalls—long before they lead to kernel panics or hardware lockups.

---

## 2. Linux Telemetry Collection & Virtual Filesystems

Linux exposes kernel internals and hardware peripheral states via virtual memory-backed filesystems:

| Filesystem Path | Monitored Subsystem | Metric Extracted | Kernel Representation |
|:---|:---|:---|:---|
| `/proc/stat` | CPU & Kernel Activity | User %, System %, IO-Wait %, Context Switches | Cumulative Jiffies / Counters |
| `/proc/loadavg` | CPU Run Queue | 1-min, 5-min, 15-min Load Averages | Exponential Decay Fixed-Point Averages |
| `/proc/meminfo` | Memory Subsystem | MemTotal, MemAvailable, Buffers, Cached, Swap | Kilobytes in Physical RAM / Swap Space |
| `/sys/class/thermal/` | Hardware Thermal Zones | Junction Temperature, Cooling Device State | Millidegrees Celsius / Governor Level |
| `/sys/class/net/` | Network Subsystem | RX/TX Dropped Packets, Overrun Errors | Hardware Ring Buffer Drop Counters |
| `/sys/bus/usb/devices/` | USB Subsystem | Connected device nodes & enumeration topology | Device Tree Nodes |

---

## 3. High-Fidelity Signal Conditioning & Feature Engineering

Raw telemetry metrics exhibit different units, ranges, and temporal dynamics. The `LinuxFeaturePreprocessor` transforms raw metrics into a clean 18-dimensional feature vector $\mathbf{x} \in \mathbb{R}^{18}$:

```
Feature Index | Feature Name          | Unit           | Description
[0]           | cpu_user_pct          | %              | CPU percentage spent in userspace processes
[1]           | cpu_system_pct        | %              | CPU percentage spent executing kernel code
[2]           | cpu_iowait_pct        | %              | CPU percentage idle waiting for disk I/O
[3]           | cpu_loadavg_1m        | Load           | 1-minute load average normalized by core count
[4]           | context_switches_sec  | switches/s     | Context switch rate across process threads
[5]           | memory_used_pct       | %              | Active RAM utilization relative to MemTotal
[6]           | swap_used_pct         | %              | Percentage of swap space allocated
[7]           | soc_max_temp_c        | °C             | Peak junction temperature across thermal zones
[8]           | throttled_state       | Integer [0-3]  | CPU governor thermal throttling level
[9]           | interrupts_sec        | IRQs/sec       | Rate of hardware and software interrupts
[10]          | procs_running         | Count          | Number of runnable processes in task queue
[11]          | procs_blocked         | Count          | Number of processes waiting in uninterruptible sleep (D-state)
[12]          | zombie_count          | Count          | Defunct child processes waiting for waitpid()
[13]          | disk_write_kb_sec     | kB/s           | Storage write throughput
[14]          | network_rx_drops      | drops/s        | Dropped packets at network interface ring buffer
[15]          | uart_errors           | errors/s       | Hardware UART framing or overrun errors
[16]          | delta_temp_dt         | °C/s           | First temporal derivative of SoC temperature
[17]          | delta_irq_dt          | IRQs/s²        | First temporal derivative of interrupt rate
```

---

## 4. Unsupervised Isolation Forest Algorithm

### 4.1 Theoretical Foundation
Unlike supervised classifiers that require massive, labeled anomaly datasets, the **Isolation Forest** exploits two quantitative properties of anomalous points:
1. **Few in number:** Anomalies represent a tiny fraction of total operational states ($< 3\%$).
2. **Attribute-divergent:** Their feature values deviate markedly from normal operational clusters.

An ensemble of $T = 100$ isolation trees ($iTrees$) is constructed by recursively partitioning the feature space with random axis-aligned splits until points are fully isolated.

The anomaly score $s(x, n)$ for an input vector $x$ over a dataset of size $n$ is defined as:

$$s(x, n) = 2^{-\frac{\mathbb{E}(h(x))}{c(n)}}$$

Where:
- $h(x)$ is the path length (number of edges traversed from root to leaf to isolate point $x$).
- $\mathbb{E}(h(x))$ is the average path length across all $100$ isolation trees.
- $c(n) = 2 \ln(n - 1) + 0.5772156649 - \frac{2(n - 1)}{n}$ is the average path length of unsuccessful searches in binary search trees.

### 4.2 Calibrated Continuous Risk Projection
To provide actionable risk metrics for system operators, the raw decision function $D(x) = -(s(x, n) - 0.5)$ is mapped to a continuous $[0.0\%, 100.0\%]$ scale via a calibrated logistic sigmoid:

$$\text{RiskScore}(x) = \frac{100.0}{1 + e^{18.0 \cdot D(x)}}$$

- **Risk < 45.0% (`HEALTHY`):** System operates within normal nominal envelope.
- **45.0% ≤ Risk < 65.0% (`ELEVATED_RISK`):** Transient load surge or moderate thermal rise.
- **Risk ≥ 65.0% (`CRITICAL_ANOMALY`):** Severe anomaly detected (e.g. interrupt storm, thermal runaway).

---

## 5. Root-Cause Diagnostic & Mitigation Playbook

| Fault Mode | Signature Features | Primary Root Cause | Automated / Operator Mitigation |
|:---|:---|:---|:---|
| **Thermal Runaway** | $T_{\text{SoC}} \ge 85^\circ\text{C}$, $\text{Throttle} \ge 2$, $\Delta T / \Delta t > 1.5$ | Heatsink dislodged, fan stall, heavy computation | Lower CPU governor max frequency, spin fan to 100% |
| **Interrupt Storm** | $\text{IRQs}/s > 50,000$, $\text{CSW}/s > 40,000$, $CPU_{\text{sys}} > 60\%$ | Spurious hardware line ringing, stuck GPIO/UART | Mask IRQ, reload peripheral kernel module |
| **Memory Thrashing** | $\%RAM > 92\%$, $\%Swap > 60\%$, Disk Write $> 25\text{MB}/s$ | Process memory leak, runaway memory buffer | Trigger cgroup memory isolation, terminate PID |
| **I/O Wait Lockup** | $\%CPU_{\text{iowait}} > 40\%$, $Procs_{\text{block}} > 10$ | Storage media degradation, saturated bus | Reschedule low-priority flush, inspect dmesg/SMART |
| **Packet Loss Surge** | $Drops_{\text{net}} > 1000/\text{sec}$ | NIC ring buffer exhaustion, port congestion | Increase ring buffer with `ethtool -G eth0 rx 4096` |
