"""
Linux Risk Classifier and Diagnostic Root-Cause Engine
Analyzes telemetry packets and Isolation Forest anomaly scores to isolate
root causes and recommend mitigation strategies for Linux system administrators.
"""

from typing import Dict, List, Optional
from telemetry.schema import LinuxTelemetryPacket


class LinuxRiskClassifier:
    """
    Automated root-cause diagnostic engine for Linux OS anomalies.
    """

    @staticmethod
    def classify_root_cause(
        packet: LinuxTelemetryPacket,
        risk_pct: float,
        is_anomaly: bool,
    ) -> Dict:
        """
        Evaluates subsystem metrics to diagnose the root cause of high risk or anomaly.
        """
        culprits: List[Dict[str, str]] = []
        mitigations: List[str] = []

        # 1. Thermal checks
        if packet.thermal.soc_max_temp_c >= 82.0 or packet.thermal.throttled_state >= 2:
            culprits.append({
                "subsystem": "THERMAL",
                "severity": "CRITICAL",
                "finding": f"SoC temperature {packet.thermal.soc_max_temp_c:.1f}°C exceeded safe envelope. Governor throttling state: {packet.thermal.throttled_state}.",
            })
            mitigations.append("Inspect thermal dissipation heatsink and cooling fans.")
            mitigations.append("Reduce CPU governor maximum frequency (/sys/devices/system/cpu/cpu*/cpufreq/).")

        # 2. Kernel Interrupt Storm
        if packet.kernel.interrupts_per_sec >= 50000.0 or packet.cpu.context_switches_per_sec >= 40000.0:
            culprits.append({
                "subsystem": "KERNEL_INTERRUPTS",
                "severity": "CRITICAL",
                "finding": f"Spurious interrupt storm detected: {packet.kernel.interrupts_per_sec:.0f} IRQs/sec (Context switches: {packet.cpu.context_switches_per_sec:.0f}/sec).",
            })
            mitigations.append("Inspect /proc/interrupts to identify faulty peripheral device driver generating high IRQ rates.")
            mitigations.append("Verify hardware line termination and pin floating status on external GPIO/UART/SPI lines.")

        # 3. Memory Pressure & Swap Thrashing
        if packet.memory.used_pct >= 90.0 or packet.memory.swap_used_pct >= 50.0:
            severity = "CRITICAL" if packet.memory.used_pct >= 95.0 else "WARNING"
            culprits.append({
                "subsystem": "MEMORY",
                "severity": severity,
                "finding": f"Memory exhaustion: {packet.memory.used_pct:.1f}% RAM used ({packet.memory.available_mb:.1f} MB available), Swap utilization: {packet.memory.swap_used_pct:.1f}%.",
            })
            mitigations.append("Identify rogue process consuming excessive RSS memory via 'ps aux --sort=-%mem'.")
            mitigations.append("Adjust vm.swappiness sysctl parameter or trigger emergency cgroup memory isolation.")

        # 4. IO Wait Bottleneck & Thrashing
        if packet.cpu.iowait_pct >= 35.0 or packet.kernel.processes_blocked >= 6:
            culprits.append({
                "subsystem": "DISK_IO",
                "severity": "CRITICAL",
                "finding": f"High IO wait latency: {packet.cpu.iowait_pct:.1f}% CPU spent in IO wait. Blocked processes in D-state: {packet.kernel.processes_blocked}.",
            })
            mitigations.append("Check disk health with smartctl and monitor iotop for saturated block storage devices.")

        # 5. Network Drop Surge
        if packet.io.network_rx_drop_count >= 500 or packet.io.network_tx_drop_count >= 500:
            culprits.append({
                "subsystem": "NETWORK",
                "severity": "WARNING",
                "finding": f"Packet drop surge: {packet.io.network_rx_drop_count} RX drops recorded in kernel network ring buffer.",
            })
            mitigations.append("Increase NIC ring buffer size via 'ethtool -G eth0 rx <size>'.")
            mitigations.append("Investigate physical Ethernet cable or network switch port congestion.")

        # 6. Peripheral Bus Faults
        if packet.buses.uart_error_count > 0:
            culprits.append({
                "subsystem": "PERIPHERAL_BUS",
                "severity": "WARNING",
                "finding": f"UART framing/overrun errors detected: {packet.buses.uart_error_count} errors.",
            })
            mitigations.append("Verify baud rate alignment, pull-up resistors, and cable shielding.")

        # Primary diagnosis summary
        if not culprits:
            if is_anomaly or risk_pct >= 50.0:
                primary_diagnosis = "Subtle multi-dimensional feature divergence detected by Isolation Forest without single threshold breach."
                mitigations.append("Review recent kernel dmesg logs and historical workload trends.")
            else:
                primary_diagnosis = "Nominal operating health across all monitored Linux subsystems."
        else:
            primary_diagnosis = culprits[0]["finding"]

        return {
            "risk_score_pct": risk_pct,
            "is_anomaly": is_anomaly,
            "primary_diagnosis": primary_diagnosis,
            "culprits": culprits,
            "recommended_mitigations": mitigations,
        }
