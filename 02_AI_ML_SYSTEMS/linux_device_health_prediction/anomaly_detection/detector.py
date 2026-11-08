"""
Linux Anomaly Detector
Wrapper combining feature preprocessing, Isolation Forest evaluation,
and stateful anomaly scoring thresholds.
"""

from typing import Dict, Tuple
import numpy as np

from feature_engineering.preprocessor import LinuxFeaturePreprocessor
from inference_engine.predictor import LinuxHealthPredictor
from telemetry.schema import LinuxTelemetryPacket


class LinuxAnomalyDetector:
    """
    Stateful anomaly detection pipeline evaluating sequential telemetry packets.
    """

    def __init__(self, model_path: str = None):
        self.preprocessor = LinuxFeaturePreprocessor()
        self.predictor = LinuxHealthPredictor(model_path=model_path)
        self.consecutive_anomalies = 0

    def process_packet(self, packet: LinuxTelemetryPacket) -> Dict:
        """
        Transforms packet, evaluates through model, and classifies state.
        """
        feature_vec = self.preprocessor.transform(packet)
        pred, risk_pct, decision_score = self.predictor.predict(feature_vec)

        if pred == -1 or risk_pct >= 55.0:
            self.consecutive_anomalies += 1
        else:
            self.consecutive_anomalies = max(0, self.consecutive_anomalies - 1)

        # Multi-tier health classification
        if risk_pct >= 65.0 or self.consecutive_anomalies >= 3:
            classification = "CRITICAL_ANOMALY"
        elif risk_pct >= 45.0 or self.consecutive_anomalies >= 1:
            classification = "ELEVATED_RISK"
        else:
            classification = "HEALTHY"

        return {
            "timestamp": packet.timestamp,
            "hostname": packet.hostname,
            "prediction": pred,
            "risk_score_pct": risk_pct,
            "decision_score": decision_score,
            "classification": classification,
            "consecutive_anomalies": self.consecutive_anomalies,
            "feature_vector": feature_vec.tolist(),
        }

    def reset(self) -> None:
        self.preprocessor.reset()
        self.consecutive_anomalies = 0
