"""
Linux Health AI Predictor & Risk Scoring Engine
Executes inference on 18-D feature vectors using the trained Isolation Forest model.
Maps decision function scores to standardized 0.0% - 100.0% Risk Indices.
"""

import os
import joblib
import numpy as np
from typing import Dict, Tuple


class LinuxHealthPredictor:
    """
    Evaluates Linux OS health telemetry against the trained baseline isolation model.
    """

    def __init__(self, model_path: str = None):
        if model_path is None:
            base_dir = os.path.dirname(os.path.abspath(__file__))
            model_path = os.path.join(base_dir, "model.joblib")

        self.model_path = model_path
        self.model = None
        self._load_model()

    def _load_model(self) -> None:
        if os.path.exists(self.model_path):
            try:
                self.model = joblib.load(self.model_path)
            except Exception as e:
                print(f"[LinuxHealthPredictor] Warning: Failed to load model: {e}")
                self.model = None
        else:
            print(f"[LinuxHealthPredictor] Model file not found at {self.model_path}")
            self.model = None

    def predict(self, feature_vector: np.ndarray) -> Tuple[int, float, float]:
        """
        Runs inference on an 18-D feature vector.
        Returns:
            (prediction, risk_score_pct, raw_decision_score)
            - prediction: +1 for Normal, -1 for Anomaly
            - risk_score_pct: 0.0% (optimal health) to 100.0% (critical failure risk)
            - raw_decision_score: isolation score (< 0 indicates outlier)
        """
        if self.model is None:
            self._load_model()
            if self.model is None:
                # Rule-based fallback if model is unbuilt
                return self._fallback_evaluate(feature_vector)

        vec = feature_vector.reshape(1, -1)
        pred = int(self.model.predict(vec)[0])
        decision_score = float(self.model.decision_function(vec)[0])

        # Convert decision function score to calibrated 0.0 - 100.0% risk
        # Decision function is roughly in [-0.25, 0.20] for scikit-learn IsolationForest
        # > +0.10: Very safe / healthy (0 - 25% risk)
        # 0.00 to +0.10: Moderate normal (25 - 49% risk)
        # -0.05 to 0.00: Borderline elevated risk (50 - 70% risk)
        # < -0.05: Severe / critical outlier (70 - 100% risk)
        sigmoid_val = 1.0 / (1.0 + np.exp(decision_score * 18.0))
        risk_pct = round(float(sigmoid_val * 100.0), 1)

        return pred, risk_pct, round(decision_score, 4)

    def _fallback_evaluate(self, vec: np.ndarray) -> Tuple[int, float, float]:
        """Rule-based heuristic fallback if model artifact is unavailable."""
        soc_temp = vec[7]
        mem_pct = vec[5]
        irq_sec = vec[9]

        if soc_temp > 85.0 or mem_pct > 92.0 or irq_sec > 60000.0:
            return -1, 88.5, -0.0950
        elif soc_temp > 75.0 or mem_pct > 80.0:
            return 1, 48.0, 0.0100
        return 1, 15.0, 0.1200
