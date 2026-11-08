"""
Risk Assessment and Root-Cause Diagnostic Classifier
Analyzes anomaly scores alongside domain-specific subsystem telemetry
to identify root-cause failure vectors and generate mitigation directives.
"""

from typing import Dict, List, Optional
from telemetry.schema import AndroidTelemetryPacket


class DeviceRiskAssessor:
    """
    Evaluates multi-domain hardware health and diagnoses the primary failure mechanism.
    """

    @staticmethod
    def assess(packet: AndroidTelemetryPacket, inference_result: Dict) -> Dict:
        status = inference_result.get("status", "HEALTHY")
        risk_pct = inference_result.get("risk_score_pct", 0.0)
        reasons: List[str] = []
        mitigations: List[str] = []

        # Domain 1: Thermal Health
        if packet.thermal.soc_temp_c >= 82.0 or packet.thermal.throttling_level >= 2:
            reasons.append(f"Excessive SoC Temperature: {packet.thermal.soc_temp_c}°C (Throttling Level {packet.thermal.throttling_level})")
            mitigations.append("Throttle max CPU clock frequency to 1.2 GHz; reduce screen brightness.")

        if packet.thermal.battery_temp_c >= 48.0:
            reasons.append(f"Dangerous Battery Temperature: {packet.thermal.battery_temp_c}°C")
            mitigations.append("Disable fast-charging circuitry; alert user to disconnect AC power.")

        # Domain 2: Memory Pressure & LMK
        if packet.memory.used_pct >= 95.0 or packet.memory.lmk_kill_count > 0:
            reasons.append(f"Critical Memory Exhaustion: {packet.memory.used_pct}% used ({packet.memory.lmk_kill_count} LMK process terminations)")
            mitigations.append("Trim background application caches; restart memory-leaking processes.")

        # Domain 3: CPU Saturation
        if packet.cpu.total_pct >= 92.0:
            top_proc = packet.cpu.top_processes[0]["name"] if packet.cpu.top_processes else "Unknown"
            reasons.append(f"High CPU Saturation: {packet.cpu.total_pct}% (Offender: {top_proc})")
            mitigations.append(f"Inspect thread state of {top_proc}; enforce background cpu.shares cgroup limit.")

        # Domain 4: Peripheral & Subsystem Stalls
        if packet.subsystems.bluetooth_drop_count >= 3:
            reasons.append(f"Frequent Bluetooth Disconnections: {packet.subsystems.bluetooth_drop_count} events")
            mitigations.append("Restart Android Bluetooth HCI daemon (`bluetoothd`).")

        if packet.subsystems.camera_frames_dropped >= 10:
            reasons.append(f"Camera Buffer Stalls: {packet.subsystems.camera_frames_dropped} dropped frames")
            mitigations.append("Reset Camera HAL surface pipeline.")

        if not reasons:
            reasons.append("All subsystem telemetry parameters operating within nominal baseline bounds.")
            mitigations.append("Maintain standard 3-second telemetry polling interval.")

        return {
            "device_id": packet.device_id,
            "timestamp": packet.timestamp,
            "health_status": status,
            "risk_score_pct": risk_pct,
            "primary_driver": reasons[0] if reasons else "Nominal",
            "diagnostics": reasons,
            "recommended_actions": mitigations,
        }
