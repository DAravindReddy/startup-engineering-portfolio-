#!/usr/bin/env python3
"""
Android Device Health Monitoring Agent Daemon
Coordinates ADB telemetry collection, feature extraction, AI inference,
and risk assessment in real-time.
"""

import argparse
import json
import os
import sys
import time

# Ensure local imports work
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from data_collection.adb_collector import ADBDeviceCollector
from feature_engineering.preprocessor import TelemetryFeaturePreprocessor
from inference_engine.predictor import AndroidHealthPredictor
from risk_assessment.risk_classifier import DeviceRiskAssessor


class AndroidHealthAgent:
    """
    Main supervisor daemon executing the predictive telemetry cycle.
    """

    def __init__(self, force_mock: bool = False, model_path: str = None):
        self.collector = ADBDeviceCollector(force_mock=force_mock)
        self.preprocessor = TelemetryFeaturePreprocessor(window_size=5)
        self.predictor = AndroidHealthPredictor(model_path=model_path)
        self.assessor = DeviceRiskAssessor()

    def step(self, inject_anomaly: str = None) -> dict:
        """
        Executes a single monitoring cycle.
        """
        # 1. Collect telemetry
        packet = self.collector.collect(inject_anomaly=inject_anomaly)

        # 2. Extract features
        features = self.preprocessor.add_packet(packet)

        # 3. Model inference
        inference = self.predictor.predict(features)

        # 4. Domain risk assessment
        assessment = self.assessor.assess(packet, inference)

        return {
            "telemetry": packet.to_dict(),
            "inference": inference,
            "assessment": assessment,
        }

    def run_loop(self, interval_sec: float = 2.0, anomaly: str = None, max_steps: int = 0):
        print("================================================================")
        print(" ANDROID PREDICTIVE DEVICE HEALTH MONITORING AGENT")
        print(f" Target Device: {self.collector.device_id} (Mode: {'LIVE' if self.collector.is_live else 'MOCK'})")
        print("================================================================")

        step_count = 0
        try:
            while True:
                step_count += 1
                result = self.step(inject_anomaly=anomaly)
                telemetry = result["telemetry"]
                inference = result["inference"]
                assessment = result["assessment"]

                print(f"\n[STEP {step_count}] Timestamp: {telemetry['timestamp']:.1f}")
                print(f"  CPU: {telemetry['cpu']['total_pct']}% | SoC Temp: {telemetry['thermal']['soc_temp_c']}°C | RAM: {telemetry['memory']['used_pct']}%")
                print(f"  AI Health Status: {inference['status']} (Risk: {inference['risk_score_pct']}%, Decision: {inference['raw_score']})")
                print(f"  Diagnosis: {assessment['primary_driver']}")

                if max_steps > 0 and step_count >= max_steps:
                    break
                time.sleep(interval_sec)
        except KeyboardInterrupt:
            print("\n[*] Health Agent terminated by operator.")


def main():
    parser = argparse.ArgumentParser(description="Android Health Agent")
    parser.add_argument("--mock", action="store_true", help="Force synthetic simulation")
    parser.add_argument("--anomaly", type=str, choices=["thermal", "memory_leak", "cpu_hog", "subsystem_stall"], help="Inject specific failure pattern")
    parser.add_argument("--interval", type=float, default=2.0, help="Sampling interval in seconds")
    parser.add_argument("--steps", type=int, default=0, help="Max steps to run (0 for infinite)")
    parser.add_argument("--demo", action="store_true", help="Run 5-step quick verification demo")
    args = parser.parse_args()

    # Automatically trigger model training if model.joblib does not exist yet
    infer_model = os.path.join(os.path.dirname(__file__), "../inference_engine/model.joblib")
    if not os.path.exists(infer_model):
        print("[*] Model binary not found. Triggering automated model training first...")
        from model_training.isolation_forest.train import main as train_main
        train_main()

    agent = AndroidHealthAgent(force_mock=args.mock or args.demo)
    steps = 5 if args.demo else args.steps
    agent.run_loop(interval_sec=args.interval, anomaly=args.anomaly, max_steps=steps)


if __name__ == "__main__":
    main()
