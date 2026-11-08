"""
Automated Test Suite for Android Device Health Prediction AI
Tests telemetry schemas, feature engineering, Isolation Forest inference,
and root-cause risk classification using Python's native unittest framework.
"""

import os
import sys
import tempfile
import unittest
import numpy as np

# Ensure local modules can be imported
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from telemetry.schema import (
    AndroidTelemetryPacket,
    CPUMetrics,
    GPUMetrics,
    ThermalMetrics,
    MemoryMetrics,
    SubsystemMetrics,
)
from data_collection.adb_collector import ADBDeviceCollector
from feature_engineering.preprocessor import TelemetryFeaturePreprocessor
from model_training.isolation_forest.train import generate_training_baseline, train_isolation_forest
from inference_engine.predictor import AndroidHealthPredictor
from risk_assessment.risk_classifier import DeviceRiskAssessor


class TestAndroidHealthPredictionAI(unittest.TestCase):

    def test_schema_serialization(self):
        collector = ADBDeviceCollector(force_mock=True)
        packet = collector.collect()

        self.assertIsNotNone(packet.device_id)
        self.assertGreaterEqual(packet.cpu.total_pct, 0.0)
        self.assertGreater(packet.thermal.soc_temp_c, 0.0)

        d = packet.to_dict()
        self.assertIn("cpu", d)
        self.assertIn("thermal", d)

        vec = packet.to_flat_feature_vector()
        self.assertEqual(len(vec), 12)
        self.assertEqual(len(AndroidTelemetryPacket.feature_names()), 12)

    def test_feature_preprocessor_derivatives(self):
        collector = ADBDeviceCollector(force_mock=True)
        preprocessor = TelemetryFeaturePreprocessor(window_size=5)

        packet1 = collector.collect()
        vec1 = preprocessor.add_packet(packet1)
        self.assertEqual(len(vec1), 14)  # 12 base + 2 temporal derivatives

        # Simulate next packet 2 seconds later with rising temperature
        packet2 = collector.collect()
        packet2.timestamp = packet1.timestamp + 2.0
        packet2.thermal.soc_temp_c = packet1.thermal.soc_temp_c + 6.0
        vec2 = preprocessor.add_packet(packet2)

        # Thermal gradient should be approx (6.0 / 2.0) = 3.0 C/sec
        self.assertAlmostEqual(float(vec2[-2]), 3.0, places=1)

    def test_model_training_and_inference(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            # 1. Train model on small baseline
            X_train = generate_training_baseline(n_samples=250)
            bundle = train_isolation_forest(X_train, n_estimators=40, contamination=0.05)

            model_file = os.path.join(tmpdir, "test_model.joblib")
            import joblib
            joblib.dump(bundle, model_file)

            # 2. Initialize inference engine
            predictor = AndroidHealthPredictor(model_path=model_file)
            collector = ADBDeviceCollector(force_mock=True)
            preprocessor = TelemetryFeaturePreprocessor(window_size=5)

            # 3. Test Normal Sample -> Should be HEALTHY or LOW RISK
            normal_pkt = collector.collect(inject_anomaly=None)
            feat_normal = preprocessor.add_packet(normal_pkt)
            res_normal = predictor.predict(feat_normal)

            self.assertIn("risk_score_pct", res_normal)
            self.assertIn("status", res_normal)
            self.assertLessEqual(res_normal["risk_score_pct"], 50.0)

            # 4. Test Extreme Thermal Anomaly -> Should be CRITICAL_ANOMALY
            thermal_pkt = collector.collect(inject_anomaly="thermal")
            feat_thermal = preprocessor.add_packet(thermal_pkt)
            res_thermal = predictor.predict(feat_thermal)

            self.assertEqual(res_thermal["is_anomaly"], -1)
            self.assertGreaterEqual(res_thermal["risk_score_pct"], 60.0)
            self.assertEqual(res_thermal["status"], "CRITICAL_ANOMALY")

    def test_risk_classifier_root_cause(self):
        collector = ADBDeviceCollector(force_mock=True)
        assessor = DeviceRiskAssessor()

        packet = collector.collect(inject_anomaly="thermal")
        inference = {
            "status": "CRITICAL_ANOMALY",
            "risk_score_pct": 88.5,
            "is_anomaly": -1,
            "raw_score": -0.18,
        }

        assessment = assessor.assess(packet, inference)
        self.assertEqual(assessment["health_status"], "CRITICAL_ANOMALY")
        self.assertEqual(assessment["risk_score_pct"], 88.5)
        self.assertIn("SoC Temperature", assessment["primary_driver"])
        self.assertGreater(len(assessment["recommended_actions"]), 0)


if __name__ == "__main__":
    unittest.main(verbosity=2)
