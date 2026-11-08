"""
Unit Tests for Linux Device Health Prediction AI
Covers procfs/sysfs parsing, telemetry collection, feature engineering,
Isolation Forest inference, and root-cause risk classification.
"""

import os
import sys
import unittest
import numpy as np

# Ensure project root is in sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from anomaly_detection.detector import LinuxAnomalyDetector
from data_collection.linux_collector import LinuxTelemetryCollector
from feature_engineering.preprocessor import LinuxFeaturePreprocessor
from inference_engine.predictor import LinuxHealthPredictor
from kernel_metrics.procfs_parser import ProcfsParser
from kernel_metrics.sysfs_parser import SysfsParser
from risk_assessment.risk_classifier import LinuxRiskClassifier
from telemetry.schema import LinuxTelemetryPacket


class TestLinuxParsers(unittest.TestCase):
    def test_procfs_parser(self):
        loadavg = ProcfsParser.parse_loadavg()
        self.assertEqual(len(loadavg), 3)
        self.assertGreaterEqual(loadavg[0], 0.0)

        mem = ProcfsParser.parse_meminfo()
        self.assertIn("total_mb", mem)
        self.assertIn("used_pct", mem)
        self.assertGreater(mem["total_mb"], 0.0)

        stat = ProcfsParser.parse_stat()
        self.assertIn("context_switches", stat)
        self.assertIn("processes_running", stat)

    def test_sysfs_parser(self):
        max_temp, zones, cooling, throttled = SysfsParser.parse_thermal()
        self.assertGreater(max_temp, 0.0)
        self.assertIsInstance(zones, list)
        self.assertIsInstance(cooling, bool)

        buses = SysfsParser.parse_subsystem_buses()
        self.assertIn("usb_device_count", buses)
        self.assertIn("i2c_bus_count", buses)


class TestLinuxTelemetryCollector(unittest.TestCase):
    def setUp(self):
        self.collector = LinuxTelemetryCollector(synthetic=True)

    def test_packet_structure(self):
        packet = self.collector.collect()
        self.assertIsInstance(packet, LinuxTelemetryPacket)
        flat = packet.to_flat_feature_vector()
        self.assertEqual(len(flat), 16)
        self.assertEqual(len(packet.feature_names()), 16)

    def test_anomaly_injection(self):
        self.collector.inject_anomaly("thermal_throttling", duration_ticks=2)
        packet = self.collector.collect()
        self.assertGreaterEqual(packet.thermal.soc_max_temp_c, 85.0)
        self.assertEqual(packet.thermal.throttled_state, 3)

        # Clear anomaly
        self.collector.clear_anomaly()
        packet_normal = self.collector.collect()
        self.assertLess(packet_normal.thermal.soc_max_temp_c, 60.0)


class TestLinuxFeaturePreprocessor(unittest.TestCase):
    def test_transform_dimensions_and_derivatives(self):
        prep = LinuxFeaturePreprocessor()
        collector = LinuxTelemetryCollector(synthetic=True)

        p1 = collector._generate_synthetic_packet(timestamp=100.0, anomaly_mode=None)
        v1 = prep.transform(p1)
        self.assertEqual(v1.shape, (18,))
        # First sample has 0 derivatives
        self.assertEqual(v1[16], 0.0)
        self.assertEqual(v1[17], 0.0)

        # Second sample after 2 seconds with higher temperature
        p2 = collector._generate_synthetic_packet(timestamp=102.0, anomaly_mode="thermal_throttling")
        v2 = prep.transform(p2)
        self.assertEqual(v2.shape, (18,))
        self.assertGreater(v2[16], 0.0)  # delta_temp_dt positive


class TestLinuxInferenceAndClassification(unittest.TestCase):
    def setUp(self):
        self.predictor = LinuxHealthPredictor()
        self.detector = LinuxAnomalyDetector()

    def test_model_prediction_nominal(self):
        prep = LinuxFeaturePreprocessor()
        collector = LinuxTelemetryCollector(synthetic=True)
        packet = collector._generate_synthetic_packet(timestamp=100.0, anomaly_mode=None)
        vec = prep.transform(packet)

        pred, risk_pct, decision_score = self.predictor.predict(vec)
        self.assertIn(pred, [-1, 1])
        self.assertGreaterEqual(risk_pct, 0.0)
        self.assertLessEqual(risk_pct, 100.0)

    def test_risk_classifier_root_cause(self):
        collector = LinuxTelemetryCollector(synthetic=True)
        # 1. Thermal
        p_thermal = collector._generate_synthetic_packet(100.0, "thermal_throttling")
        diag_thermal = LinuxRiskClassifier.classify_root_cause(p_thermal, 75.0, is_anomaly=True)
        self.assertTrue(any(c["subsystem"] == "THERMAL" for c in diag_thermal["culprits"]))

        # 2. Interrupt storm
        p_irq = collector._generate_synthetic_packet(100.0, "interrupt_storm")
        diag_irq = LinuxRiskClassifier.classify_root_cause(p_irq, 80.0, is_anomaly=True)
        self.assertTrue(any(c["subsystem"] == "KERNEL_INTERRUPTS" for c in diag_irq["culprits"]))

        # 3. Memory exhaustion
        p_mem = collector._generate_synthetic_packet(100.0, "memory_exhaustion")
        diag_mem = LinuxRiskClassifier.classify_root_cause(p_mem, 70.0, is_anomaly=True)
        self.assertTrue(any(c["subsystem"] == "MEMORY" for c in diag_mem["culprits"]))


if __name__ == "__main__":
    unittest.main()
