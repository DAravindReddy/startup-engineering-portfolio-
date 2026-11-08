"""
Telemetry Feature Engineering & Time-Series Preprocessor
Calculates rolling statistical windows, rate-of-change derivatives,
and normalized feature vectors.
"""

from collections import deque
from typing import Dict, List, Optional
import numpy as np
from telemetry.schema import AndroidTelemetryPacket


class TelemetryFeaturePreprocessor:
    """
    Transforms raw Android telemetry streams into enriched feature vectors
    incorporating temporal derivatives (thermal gradients and memory leak velocities).
    """

    def __init__(self, window_size: int = 5):
        self.window_size = window_size
        self.history: deque = deque(maxlen=window_size)
        self.feature_means: Optional[np.ndarray] = None
        self.feature_stds: Optional[np.ndarray] = None

    def add_packet(self, packet: AndroidTelemetryPacket) -> np.ndarray:
        """
        Appends packet to window and returns computed feature vector.
        """
        self.history.append(packet)
        return self.extract_features(packet)

    def extract_features(self, packet: AndroidTelemetryPacket) -> np.ndarray:
        """
        Extracts base features plus temporal rate-of-change metrics.
        """
        base_features = packet.to_flat_feature_vector()

        # Compute dynamic temporal derivatives if history has >= 2 frames
        thermal_gradient = 0.0
        memory_velocity = 0.0

        if len(self.history) >= 2:
            prev_packet = self.history[-2]
            dt = max(0.1, packet.timestamp - prev_packet.timestamp)
            thermal_gradient = (packet.thermal.soc_temp_c - prev_packet.thermal.soc_temp_c) / dt
            memory_velocity = (packet.memory.used_mb - prev_packet.memory.used_mb) / dt

        # Enriched 14-dimensional vector
        enriched = base_features + [float(thermal_gradient), float(memory_velocity)]
        return np.array(enriched, dtype=np.float32)

    @staticmethod
    def get_enriched_feature_names() -> List[str]:
        return AndroidTelemetryPacket.feature_names() + [
            "thermal_gradient_c_per_sec",
            "memory_velocity_mb_per_sec",
        ]
