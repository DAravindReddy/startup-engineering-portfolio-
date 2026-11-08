"""
Low-Latency Real-Time Telemetry Inference Engine
Evaluates streaming feature vectors against trained Isolation Forest models
and translates raw scores into continuous risk percentages.
"""

import os
import sys
from typing import Dict, List, Optional, Union
import numpy as np
import joblib

# Ensure local imports work
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from telemetry.schema import AndroidTelemetryPacket


class AndroidHealthPredictor:
    """
    Production inference engine for on-device or edge gateway execution.
    """

    def __init__(self, model_path: Optional[str] = None):
        if not model_path:
            model_path = os.path.join(os.path.dirname(__file__), "model.joblib")
        self.model_path = model_path
        self.model = None
        self.feature_names = []
        self.score_min = -0.5
        self.score_max = 0.5
        self._load_model()

    def _load_model(self):
        if not os.path.exists(self.model_path):
            raise FileNotFoundError(
                f"Model artifact not found at {self.model_path}. "
                "Run model_training/isolation_forest/train.py first."
            )
        bundle = joblib.load(self.model_path)
        self.model = bundle["model"]
        self.feature_names = bundle.get("feature_names", [])
        self.score_min = bundle.get("score_min", -0.5)
        self.score_max = bundle.get("score_max", 0.5)

    def predict(self, feature_vector: Union[np.ndarray, List[float]]) -> Dict[str, Union[float, str, int]]:
        """
        Runs inference on an enriched 14-dimensional feature vector.
        """
        X = np.asarray(feature_vector, dtype=np.float32).reshape(1, -1)
        pred = int(self.model.predict(X)[0])
        raw_score = float(self.model.decision_function(X)[0])

        # Calibrated Risk Mapping
        # Raw score > 0.12 => Healthy, low risk
        # 0.0 <= Raw score <= 0.12 => Elevated risk (boundary state)
        # Raw score < 0.0 => Definite anomaly
        if raw_score >= 0.12:
            risk_pct = max(0.0, min(25.0, (0.35 - raw_score) * 80.0))
            status = "HEALTHY"
        elif raw_score >= 0.0:
            risk_pct = 25.0 + ((0.12 - raw_score) / 0.12) * 35.0
            status = "ELEVATED_RISK"
        else:
            severity = min(1.0, abs(raw_score) / 0.25)
            risk_pct = 60.0 + (severity * 40.0)
            status = "CRITICAL_ANOMALY"

        return {
            "is_anomaly": pred,
            "raw_score": round(raw_score, 4),
            "risk_score_pct": round(float(risk_pct), 1),
            "status": status,
        }
