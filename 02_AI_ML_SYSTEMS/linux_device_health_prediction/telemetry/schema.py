"""
Linux OS Telemetry Data Schema
Defines structured dataclasses for Linux operating system metrics,
procfs statistics, sysfs thermal zones, and peripheral subsystem counters.
"""

from dataclasses import asdict, dataclass, field
from typing import Dict, List, Optional
import time


@dataclass
class LinuxCPUMetrics:
    user_pct: float
    system_pct: float
    iowait_pct: float
    idle_pct: float
    loadavg_1m: float
    loadavg_5m: float
    loadavg_15m: float
    context_switches_per_sec: float


@dataclass
class LinuxMemoryMetrics:
    total_mb: float
    available_mb: float
    used_mb: float
    used_pct: float
    buffers_mb: float
    cached_mb: float
    swap_total_mb: float
    swap_used_mb: float
    swap_used_pct: float


@dataclass
class LinuxThermalMetrics:
    soc_max_temp_c: float
    thermal_zones: List[float] = field(default_factory=list)
    cooling_active: bool = False
    throttled_state: int = 0


@dataclass
class LinuxKernelMetrics:
    interrupts_per_sec: float
    processes_running: int
    processes_blocked: int
    zombie_count: int


@dataclass
class LinuxIOMetrics:
    disk_read_kb_per_sec: float
    disk_write_kb_per_sec: float
    network_rx_kb_per_sec: float
    network_tx_kb_per_sec: float
    network_rx_drop_count: int
    network_tx_drop_count: int


@dataclass
class LinuxBusMetrics:
    usb_device_count: int
    i2c_bus_count: int
    spi_devices_active: int
    uart_error_count: int


@dataclass
class LinuxTelemetryPacket:
    timestamp: float
    hostname: str
    cpu: LinuxCPUMetrics
    memory: LinuxMemoryMetrics
    thermal: LinuxThermalMetrics
    kernel: LinuxKernelMetrics
    io: LinuxIOMetrics
    buses: LinuxBusMetrics

    def to_dict(self) -> Dict:
        return asdict(self)

    def to_flat_feature_vector(self) -> List[float]:
        """Flattens hierarchical telemetry into a 16-D base feature vector."""
        return [
            self.cpu.user_pct,
            self.cpu.system_pct,
            self.cpu.iowait_pct,
            self.cpu.loadavg_1m,
            self.cpu.context_switches_per_sec,
            self.memory.used_pct,
            self.memory.swap_used_pct,
            self.thermal.soc_max_temp_c,
            float(self.thermal.throttled_state),
            self.kernel.interrupts_per_sec,
            float(self.kernel.processes_running),
            float(self.kernel.processes_blocked),
            float(self.kernel.zombie_count),
            self.io.disk_write_kb_per_sec,
            float(self.io.network_rx_drop_count),
            float(self.buses.uart_error_count),
        ]

    @staticmethod
    def feature_names() -> List[str]:
        return [
            "cpu_user_pct",
            "cpu_system_pct",
            "cpu_iowait_pct",
            "cpu_loadavg_1m",
            "context_switches_sec",
            "memory_used_pct",
            "swap_used_pct",
            "soc_max_temp_c",
            "throttled_state",
            "interrupts_sec",
            "procs_running",
            "procs_blocked",
            "zombie_count",
            "disk_write_kb_sec",
            "network_rx_drops",
            "uart_errors",
        ]
