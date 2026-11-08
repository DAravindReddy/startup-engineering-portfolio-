"""
Isolation Forest Model Training Pipeline
Trains an unsupervised tree ensemble on normal Android operational telemetry
and serializes model artifacts for real-time inference.
"""

import os
import sys
import numpy as np
from sklearn.ensemble import IsolationForest
import joblib

# Ensure local imports work
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../..")))

from data_collection.adb_collector import ADBDeviceCollector
from feature_engineering.preprocessor import TelemetryFeaturePreprocessor


def generate_training_baseline(n_samples: int = 1200) -> np.ndarray:
    """
    Generates synthetic baseline traces representing diverse healthy Android states:
    Idle standby, background audio, web navigation, and video playback.
    """
    collector = ADBDeviceCollector(force_mock=True)
    preprocessor = TelemetryFeaturePreprocessor(window_size=5)
    features_list = []

    print(f"[*] Generating {n_samples} normal Android telemetry training vectors...")
    for _ in range(n_samples):
        # Generate normal operation (anomaly=None)
        packet = collector.collect(inject_anomaly=None)
        feat = preprocessor.add_packet(packet)
        features_list.append(feat)

    X = np.array(features_list, dtype=np.float32)
    return X


def train_isolation_forest(
    X_train: np.ndarray,
    n_estimators: int = 100,
    contamination: float = 0.04,
    random_state: int = 42,
) -> dict:
    """
    Trains scikit-learn Isolation Forest on baseline healthy telemetry.
    """
    print(f"[*] Training Isolation Forest (n_estimators={n_estimators}, contamination={contamination})...")
    model = IsolationForest(
        n_estimators=n_estimators,
        contamination=contamination,
        random_state=random_state,
        n_jobs=1,
    )
    model.fit(X_train)

    # Evaluate decision function bounds on training data
    scores = model.decision_function(X_train)
    score_min = float(np.min(scores))
    score_max = float(np.max(scores))
    score_mean = float(np.mean(scores))

    print(f"[+] Model training complete!")
    print(f"    Decision Function Range: [{score_min:.4f}, {score_max:.4f}] (Mean: {score_mean:.4f})")

    feature_names = TelemetryFeaturePreprocessor.get_enriched_feature_names()
    bundle = {
        "model": model,
        "feature_names": feature_names,
        "score_min": score_min,
        "score_max": score_max,
        "contamination": contamination,
        "n_estimators": n_estimators,
    }
    return bundle


def main():
    X_train = generate_training_baseline(n_samples=1200)
    bundle = train_isolation_forest(X_train)

    # Save artifact in model_training and copy to inference_engine
    out_dir = os.path.dirname(__file__)
    model_path = os.path.join(out_dir, "model.joblib")
    joblib.dump(bundle, model_path)
    print(f"[+] Serialized model artifact saved to: {model_path}")

    infer_path = os.path.abspath(os.path.join(out_dir, "../../inference_engine/model.joblib"))
    joblib.dump(bundle, infer_path)
    print(f"[+] Synced model artifact to inference engine: {infer_path}")


if __name__ == "__main__":
    main()
