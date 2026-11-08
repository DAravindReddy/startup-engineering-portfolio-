"""
Linux Feature Engineering & Preprocessor
Extracts continuous telemetry feature vectors and computes temporal rates of change
(derivatives) across consecutive snapshots for accurate anomaly detection.
"""

from collections import deque
from typing import List, Optional, Tuple
import numpy as np

from telemetry.schema import LinuxTelemetryPacket


class LinuxFeaturePreprocessor:
    """
    Transforms raw LinuxTelemetryPackets into stationary, scaled 18-dimensional feature vectors.
    Computes time-delta derivatives for temperature rise and interrupt surges.
    """

    def __init__(self, history_len: int = 5):
        self.history = deque(maxlen=history_len)
        self._prev_packet: Optional[LinuxTelemetryPacket] = None

    def transform(self, packet: LinuxTelemetryPacket) -> np.ndarray:
        """
        Transforms a telemetry packet into an 18-dimensional numpy vector.
        Features 0-15: Static base features.
        Feature 16: delta_temp_dt (°C/sec).
        Feature 17: delta_irq_dt (Interrupts/sec²).
        """
        base_features = packet.to_flat_feature_vector()

        delta_temp_dt = 0.0
        delta_irq_dt = 0.0

        if self._prev_packet is not None:
            dt = max(0.1, packet.timestamp - self._prev_packet.timestamp)
            delta_temp = packet.thermal.soc_max_temp_c - self._prev_packet.thermal.soc_max_temp_c
            delta_irq = packet.kernel.interrupts_per_sec - self._prev_packet.kernel.interrupts_per_sec
            delta_temp_dt = delta_temp / dt
            delta_irq_dt = delta_irq / dt

        self._prev_packet = packet
        full_vector = base_features + [delta_temp_dt, delta_irq_dt]
        self.history.append(full_vector)
        return np.array(full_vector, dtype=np.float32)

    @staticmethod
    def feature_names() -> List[str]:
        return LinuxTelemetryPacket.feature_names() + [
            "delta_temp_dt",
            "delta_irq_dt",
        ]

    def reset(self) -> None:
        """Resets the rolling preprocessor history."""
        self.history.clear()
        self._prev_packet = None
