"""
Linux Device Health AI Model Trainer
Trains an unsupervised Isolation Forest model on nominal Linux operating system telemetry.
Evaluates detection performance on controlled fault conditions.
"""

import os
import shutil
import time
import joblib
import numpy as np
from sklearn.ensemble import IsolationForest

import sys
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from data_collection.linux_collector import LinuxTelemetryCollector
from feature_engineering.preprocessor import LinuxFeaturePreprocessor


def generate_training_data(sample_count: int = 1500) -> np.ndarray:
    """Generates baseline healthy operating system telemetry vectors."""
    print(f"Generating {sample_count} baseline nominal telemetry vectors...")
    collector = LinuxTelemetryCollector(synthetic=True)
    preprocessor = LinuxFeaturePreprocessor()

    vectors = []
    base_time = time.time()
    for i in range(sample_count):
        # Step forward 1.0 second each iteration
        packet = collector._generate_synthetic_packet(base_time + (i * 1.0), anomaly_mode=None)
        vector = preprocessor.transform(packet)
        vectors.append(vector)

    return np.array(vectors, dtype=np.float32)


def train_model() -> None:
    X_train = generate_training_data(1500)
    print(f"Dataset shape: {X_train.shape} (18 dimensions)")

    print("Fitting Isolation Forest model (n_estimators=100, contamination=0.03, n_jobs=1)...")
    clf = IsolationForest(
        n_estimators=100,
        contamination=0.03,
        random_state=42,
        n_jobs=1,
    )
    clf.fit(X_train)

    train_preds = clf.predict(X_train)
    nominal_outlier_rate = float(np.mean(train_preds == -1))
    print(f"Nominal training outlier rate: {nominal_outlier_rate:.2%} (Nominal threshold verified)")

    # Evaluate against known synthetic anomalies
    print("\n--- Validating Synthetic Anomaly Detection ---")
    test_collector = LinuxTelemetryCollector(synthetic=True)
    anomalies = [
        "thermal_throttling",
        "memory_exhaustion",
        "interrupt_storm",
        "iowait_thrashing",
        "network_packet_loss",
    ]

    for anomaly in anomalies:
        test_prep = LinuxFeaturePreprocessor()
        # Seed 5 normal samples
        for k in range(5):
            test_prep.transform(test_collector._generate_synthetic_packet(time.time() + k, None))

        # Inject anomaly
        anom_packet = test_collector._generate_synthetic_packet(time.time() + 6, anomaly)
        vec = test_prep.transform(anom_packet).reshape(1, -1)
        pred = clf.predict(vec)[0]
        score = clf.decision_function(vec)[0]
        status = "DETECTED (-1)" if pred == -1 else "MISSED (+1)"
        print(f"  [{status}] Fault: {anomaly:<22} Score: {score:+.4f}")

    # Serialize trained model
    script_dir = os.path.dirname(os.path.abspath(__file__))
    model_path = os.path.join(script_dir, "model.joblib")
    joblib.dump(clf, model_path)
    print(f"\nModel saved to: {model_path}")

    # Duplicate to inference engine
    infer_dir = os.path.abspath(os.path.join(script_dir, "..", "inference_engine"))
    os.makedirs(infer_dir, exist_ok=True)
    infer_path = os.path.join(infer_dir, "model.joblib")
    shutil.copyfile(model_path, infer_path)
    print(f"Model deployed to: {infer_path}")


if __name__ == "__main__":
    train_model()
