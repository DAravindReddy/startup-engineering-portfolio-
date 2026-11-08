"""
Linux Health Monitoring Agent Daemon
Autonomous edge service collecting telemetry, running Isolation Forest anomaly inference,
and dispatching real-time health alerts to logs and web dashboards.
"""

import argparse
import json
import os
import sys
import time

# Ensure project root is in sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from anomaly_detection.detector import LinuxAnomalyDetector
from data_collection.linux_collector import LinuxTelemetryCollector
from risk_assessment.risk_classifier import LinuxRiskClassifier


class LinuxHealthAgent:
    """
    Background daemon running periodic telemetry cycles and root-cause risk diagnosis.
    """

    def __init__(self, synthetic: bool = False, model_path: str = None):
        self.collector = LinuxTelemetryCollector(synthetic=synthetic)
        self.detector = LinuxAnomalyDetector(model_path=model_path)
        self.history = []

    def run_cycle(self) -> dict:
        """Executes a single monitoring cycle."""
        packet = self.collector.collect()
        detection_result = self.detector.process_packet(packet)
        diagnosis = LinuxRiskClassifier.classify_root_cause(
            packet=packet,
            risk_pct=detection_result["risk_score_pct"],
            is_anomaly=(detection_result["prediction"] == -1),
        )

        record = {
            "timestamp": packet.timestamp,
            "hostname": packet.hostname,
            "telemetry": packet.to_dict(),
            "detection": detection_result,
            "diagnosis": diagnosis,
        }
        self.history.append(record)
        if len(self.history) > 100:
            self.history.pop(0)

        return record

    def run_loop(self, interval_sec: float = 2.0, max_iterations: int = 0) -> None:
        """Runs the monitoring loop."""
        iteration = 0
        print(f"[*] Linux Health Agent started on {self.collector.hostname} (Interval: {interval_sec}s)...")
        try:
            while True:
                iteration += 1
                record = self.run_cycle()
                det = record["detection"]
                diag = record["diagnosis"]
                tel = record["telemetry"]

                status_color = "[HEALTHY]"
                if det["classification"] == "CRITICAL_ANOMALY":
                    status_color = "[CRITICAL ANOMALY]"
                elif det["classification"] == "ELEVATED_RISK":
                    status_color = "[ELEVATED RISK]"

                print(
                    f"[{time.strftime('%H:%M:%S', time.localtime(record['timestamp']))}] "
                    f"{status_color:<18} "
                    f"Risk: {det['risk_score_pct']:5.1f}% | "
                    f"CPU: {tel['cpu']['user_pct'] + tel['cpu']['system_pct']:4.1f}% | "
                    f"RAM: {tel['memory']['used_pct']:4.1f}% | "
                    f"SoC Temp: {tel['thermal']['soc_max_temp_c']:4.1f}°C | "
                    f"IRQs: {tel['kernel']['interrupts_per_sec']:6.0f}/s"
                )
                if det["classification"] != "HEALTHY":
                    print(f"    --> Diagnosis: {diag['primary_diagnosis']}")

                if max_iterations > 0 and iteration >= max_iterations:
                    break
                time.sleep(interval_sec)
        except KeyboardInterrupt:
            print("\n[*] Linux Health Agent stopped by user.")


def main():
    parser = argparse.ArgumentParser(description="Linux Device Health AI Monitoring Agent")
    parser.add_argument("--synthetic", action="store_true", help="Force synthetic telemetry simulation")
    parser.add_argument("--interval", type=float, default=2.0, help="Sampling interval in seconds")
    parser.add_argument("--iterations", type=int, default=0, help="Number of cycles to run (0 = infinite)")
    parser.add_argument("--anomaly", type=str, choices=[
        "thermal_throttling", "memory_exhaustion", "interrupt_storm", "iowait_thrashing", "network_packet_loss"
    ], help="Inject a synthetic fault condition")
    parser.add_argument("--demo", action="store_true", help="Execute 5-step automated demo cycle")
    args = parser.parse_args()

    agent = LinuxHealthAgent(synthetic=args.synthetic or args.demo or bool(args.anomaly))

    if args.demo:
        print("=== Running Linux Device Health AI 5-Step Demo ===")
        print("Step 1: Baseline nominal sampling...")
        agent.run_loop(interval_sec=0.5, max_iterations=2)

        print("\nStep 2: Injecting 'interrupt_storm' anomaly...")
        agent.collector.inject_anomaly("interrupt_storm", duration_ticks=3)
        agent.run_loop(interval_sec=0.5, max_iterations=3)

        print("\nStep 3: Returning to nominal recovery...")
        agent.collector.clear_anomaly()
        agent.run_loop(interval_sec=0.5, max_iterations=2)
        print("=== Demo Complete ===")
        return

    if args.anomaly:
        print(f"[*] Injecting active fault: {args.anomaly}")
        agent.collector.inject_anomaly(args.anomaly, duration_ticks=max(10, args.iterations))

    agent.run_loop(interval_sec=args.interval, max_iterations=args.iterations)


if __name__ == "__main__":
    main()
