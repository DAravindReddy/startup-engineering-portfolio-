"""
Isolation Forest Anomaly Detection Wrapper
Provides standardized training, anomaly scoring, risk classification,
and serialization for Edge AI predictive monitoring.
"""

import json
import os
from typing import Dict, List, Optional, Tuple, Union
import numpy as np
from sklearn.ensemble import IsolationForest
import joblib


class IsolationForestHealthPredictor:
    """
    Standardized Isolation Forest wrapper for hardware and system telemetry.
    Translates raw decision function scores into intuitive 0.0 - 100.0% Risk Scores
    and categorical Health Statuses: HEALTHY, ELEVATED_RISK, CRITICAL_ANOMALY.
    """

    def __init__(
        self,
        n_estimators: int = 100,
        contamination: float = 0.05,
        random_state: int = 42,
    ):
        self.n_estimators = n_estimators
        self.contamination = contamination
        self.random_state = random_state
        self.model = IsolationForest(
            n_estimators=self.n_estimators,
            contamination=self.contamination,
            random_state=self.random_state,
            n_jobs=1,
        )
        self.is_fitted = False
        self.feature_names: List[str] = []
        self.score_min = -0.5
        self.score_max = 0.5

    def fit(self, X: np.ndarray, feature_names: Optional[List[str]] = None) -> "IsolationForestHealthPredictor":
        """
        Fits the Isolation Forest model on normal telemetry training data.
        """
        if feature_names:
            self.feature_names = feature_names
        elif hasattr(X, "columns"):
            self.feature_names = list(X.columns)
            X = X.values

        self.model.fit(X)
        self.is_fitted = True

        # Calibrate baseline scores
        scores = self.model.decision_function(X)
        self.score_min = float(np.min(scores))
        self.score_max = float(np.max(scores))
        return self

    def predict_risk(self, sample: Union[np.ndarray, List[float]]) -> Dict[str, Union[float, str, int]]:
        """
        Evaluates a single telemetry feature vector and returns risk metrics.
        
        Returns:
            dict containing:
                - is_anomaly (int): 1 if normal, -1 if anomalous
                - raw_score (float): raw scikit-learn decision function score
                - risk_score_pct (float): calibrated risk percentage 0.0% to 100.0%
                - status (str): HEALTHY, ELEVATED_RISK, or CRITICAL_ANOMALY
        """
        if not self.is_fitted:
            raise RuntimeError("Model must be fitted before running prediction.")

        sample_arr = np.asarray(sample).reshape(1, -1)
        pred = int(self.model.predict(sample_arr)[0])
        raw_score = float(self.model.decision_function(sample_arr)[0])

        # Normalize score into Risk % (Lower decision score -> Higher anomaly risk)
        # Decision function: positive is normal, negative is anomalous
        if raw_score >= 0.15:
            risk_pct = max(0.0, min(25.0, (0.35 - raw_score) * 100.0))
            status = "HEALTHY"
        elif raw_score >= 0.0:
            risk_pct = 25.0 + ((0.15 - raw_score) / 0.15) * 35.0
            status = "ELEVATED_RISK"
        else:
            # Negative scores denote definite anomalies
            severity = min(1.0, abs(raw_score) / 0.25)
            risk_pct = 60.0 + (severity * 40.0)
            status = "CRITICAL_ANOMALY"

        return {
            "is_anomaly": pred,
            "raw_score": round(raw_score, 4),
            "risk_score_pct": round(float(risk_pct), 1),
            "status": status,
        }

    def save_model(self, file_path: str):
        """Saves model binary and metadata."""
        os.makedirs(os.path.dirname(file_path), exist_ok=True)
        meta = {
            "feature_names": self.feature_names,
            "score_min": self.score_min,
            "score_max": self.score_max,
            "contamination": self.contamination,
            "n_estimators": self.n_estimators,
        }
        joblib.dump({"model": self.model, "meta": meta}, file_path)

    @classmethod
    def load_model(cls, file_path: str) -> "IsolationForestHealthPredictor":
        """Loads serialized model binary and metadata."""
        bundle = joblib.load(file_path)
        meta = bundle["meta"]
        instance = cls(
            n_estimators=meta.get("n_estimators", 100),
            contamination=meta.get("contamination", 0.05),
        )
        instance.model = bundle["model"]
        instance.feature_names = meta.get("feature_names", [])
        instance.score_min = meta.get("score_min", -0.5)
        instance.score_max = meta.get("score_max", 0.5)
        instance.is_fitted = True
        return instance
