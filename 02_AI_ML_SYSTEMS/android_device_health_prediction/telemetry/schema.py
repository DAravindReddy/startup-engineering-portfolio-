"""
Android Telemetry Data Schema
Structured definitions for device operating metrics and subsystem telemetry.
"""

from dataclasses import asdict, dataclass, field
from typing import Dict, List, Optional
import time


@dataclass
class CPUMetrics:
    user_pct: float
    kernel_pct: float
    total_pct: float
    frequency_mhz: float
    top_processes: List[Dict[str, float]] = field(default_factory=list)


@dataclass
class GPUMetrics:
    utilization_pct: float
    frequency_mhz: float
    frame_drop_count: int


@dataclass
class ThermalMetrics:
    soc_temp_c: float
    battery_temp_c: float
    throttling_level: int  # 0=None, 1=Light, 2=Moderate, 3=Severe, 4=Critical


@dataclass
class MemoryMetrics:
    total_mb: float
    used_mb: float
    free_mb: float
    used_pct: float
    lmk_kill_count: int  # Low Memory Killer events


@dataclass
class SubsystemMetrics:
    bluetooth_connected: bool
    bluetooth_drop_count: int
    camera_frames_dropped: int
    camera_active: bool
    media_codec_stalls: int


@dataclass
class AndroidTelemetryPacket:
    timestamp: float
    device_id: str
    cpu: CPUMetrics
    gpu: GPUMetrics
    thermal: ThermalMetrics
    memory: MemoryMetrics
    subsystems: SubsystemMetrics

    def to_dict(self) -> Dict:
        return asdict(self)

    def to_flat_feature_vector(self) -> List[float]:
        """Flattens hierarchical metrics into a numerical feature vector for ML inference."""
        return [
            self.cpu.total_pct,
            self.cpu.user_pct,
            self.cpu.kernel_pct,
            self.gpu.utilization_pct,
            self.thermal.soc_temp_c,
            self.thermal.battery_temp_c,
            float(self.thermal.throttling_level),
            self.memory.used_pct,
            float(self.memory.lmk_kill_count),
            float(self.subsystems.bluetooth_drop_count),
            float(self.subsystems.camera_frames_dropped),
            float(self.subsystems.media_codec_stalls),
        ]

    @staticmethod
    def feature_names() -> List[str]:
        return [
            "cpu_total_pct",
            "cpu_user_pct",
            "cpu_kernel_pct",
            "gpu_util_pct",
            "soc_temp_c",
            "battery_temp_c",
            "thermal_throttling_level",
            "memory_used_pct",
            "lmk_kill_count",
            "bluetooth_drops",
            "camera_dropped_frames",
            "media_codec_stalls",
        ]
