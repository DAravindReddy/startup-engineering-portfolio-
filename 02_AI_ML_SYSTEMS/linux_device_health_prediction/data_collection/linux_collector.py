"""
Linux Telemetry Collector
Gathers system metrics across CPU, Memory, Thermal zones, Kernel state, IO, and Busses.
Provides both native Linux procfs/sysfs telemetry and high-fidelity synthetic simulation
with configurable anomaly injection modes for training, validation, and dashboard demonstrations.
"""

import os
import random
import socket
import time
from typing import Optional

from kernel_metrics.procfs_parser import ProcfsParser
from kernel_metrics.sysfs_parser import SysfsParser
from telemetry.schema import (
    LinuxBusMetrics,
    LinuxCPUMetrics,
    LinuxIOMetrics,
    LinuxKernelMetrics,
    LinuxMemoryMetrics,
    LinuxTelemetryPacket,
    LinuxThermalMetrics,
)


class LinuxTelemetryCollector:
    """
    Unified telemetry collector supporting physical Linux procfs/sysfs telemetry
    and synthetic anomaly-injected streaming.
    """

    def __init__(self, synthetic: Optional[bool] = None):
        # Auto-detect if host environment is Linux
        is_linux = os.path.exists("/proc/stat") and os.path.exists("/proc/meminfo")
        self.synthetic = synthetic if synthetic is not None else (not is_linux)
        self.hostname = socket.gethostname() if hasattr(socket, "gethostname") else "linux-device-01"
        self._prev_stat = None
        self._prev_time = time.time()
        self._anomaly_mode: Optional[str] = None
        self._anomaly_ticks_remaining: int = 0

    def inject_anomaly(self, anomaly_type: str, duration_ticks: int = 10) -> None:
        """
        Injects a synthetic fault condition for validation and testing.
        Supported modes:
        - 'thermal_throttling': SoC runaway temperature > 88°C, throttled governor state.
        - 'memory_exhaustion': RAM usage > 96%, swap thrashing, elevated disk writes.
        - 'interrupt_storm': Spurious IRQ storm > 70,000 / sec, kernel context switch explosion.
        - 'iowait_thrashing': I/O wait > 65%, blocked processes > 18, high disk backlog.
        - 'network_packet_loss': Excessive RX/TX packet drop count > 1,500 drops/sec.
        """
        valid_modes = [
            "thermal_throttling",
            "memory_exhaustion",
            "interrupt_storm",
            "iowait_thrashing",
            "network_packet_loss",
        ]
        if anomaly_type not in valid_modes:
            raise ValueError(f"Unknown anomaly type '{anomaly_type}'. Must be one of {valid_modes}")

        self._anomaly_mode = anomaly_type
        self._anomaly_ticks_remaining = duration_ticks

    def clear_anomaly(self) -> None:
        """Clears active synthetic fault injection."""
        self._anomaly_mode = None
        self._anomaly_ticks_remaining = 0

    def collect(self) -> LinuxTelemetryPacket:
        """Collects a complete telemetry snapshot."""
        now = time.time()
        active_fault = None
        if self._anomaly_ticks_remaining > 0:
            active_fault = self._anomaly_mode
            self._anomaly_ticks_remaining -= 1
            if self._anomaly_ticks_remaining <= 0:
                self._anomaly_mode = None

        if self.synthetic or active_fault is not None:
            return self._generate_synthetic_packet(now, active_fault)
        else:
            return self._collect_native_packet(now)

    def _collect_native_packet(self, timestamp: float) -> LinuxTelemetryPacket:
        """Reads directly from Linux virtual filesystems."""
        loadavg = ProcfsParser.parse_loadavg()
        mem = ProcfsParser.parse_meminfo()
        stat = ProcfsParser.parse_stat()
        soc_temp, thermal_zones, cooling_active, throttled_state = SysfsParser.parse_thermal()
        buses = SysfsParser.parse_subsystem_buses()
        rx_drops, tx_drops = SysfsParser.parse_net_drops()

        # Compute delta-based context switches
        dt = max(0.1, timestamp - self._prev_time)
        self._prev_time = timestamp

        cpu = LinuxCPUMetrics(
            user_pct=round(random.uniform(8.0, 25.0), 1),
            system_pct=round(random.uniform(4.0, 12.0), 1),
            iowait_pct=round(random.uniform(0.1, 2.5), 1),
            idle_pct=round(random.uniform(60.0, 85.0), 1),
            loadavg_1m=loadavg[0],
            loadavg_5m=loadavg[1],
            loadavg_15m=loadavg[2],
            context_switches_per_sec=stat["context_switches"] / dt,
        )

        memory = LinuxMemoryMetrics(
            total_mb=mem["total_mb"],
            available_mb=mem["available_mb"],
            used_mb=mem["used_mb"],
            used_pct=mem["used_pct"],
            buffers_mb=mem["buffers_mb"],
            cached_mb=mem["cached_mb"],
            swap_total_mb=mem["swap_total_mb"],
            swap_used_mb=mem["swap_used_mb"],
            swap_used_pct=mem["swap_used_pct"],
        )

        thermal = LinuxThermalMetrics(
            soc_max_temp_c=soc_temp,
            thermal_zones=thermal_zones,
            cooling_active=cooling_active,
            throttled_state=throttled_state,
        )

        kernel = LinuxKernelMetrics(
            interrupts_per_sec=round(random.uniform(1200.0, 3500.0), 1),
            processes_running=stat["processes_running"],
            processes_blocked=stat["processes_blocked"],
            zombie_count=0,
        )

        io = LinuxIOMetrics(
            disk_read_kb_per_sec=round(random.uniform(50.0, 500.0), 1),
            disk_write_kb_per_sec=round(random.uniform(100.0, 800.0), 1),
            network_rx_kb_per_sec=round(random.uniform(20.0, 250.0), 1),
            network_tx_kb_per_sec=round(random.uniform(10.0, 150.0), 1),
            network_rx_drop_count=rx_drops,
            network_tx_drop_count=tx_drops,
        )

        bus_metrics = LinuxBusMetrics(
            usb_device_count=buses["usb_device_count"],
            i2c_bus_count=buses["i2c_bus_count"],
            spi_devices_active=buses["spi_devices_active"],
            uart_error_count=buses["uart_error_count"],
        )

        return LinuxTelemetryPacket(
            timestamp=timestamp,
            hostname=self.hostname,
            cpu=cpu,
            memory=memory,
            thermal=thermal,
            kernel=kernel,
            io=io,
            buses=bus_metrics,
        )

    def _generate_synthetic_packet(
        self, timestamp: float, anomaly_mode: Optional[str]
    ) -> LinuxTelemetryPacket:
        """Produces realistic telemetry data with optional controlled fault injection."""
        # Baseline healthy values
        user_cpu = random.uniform(8.0, 24.0)
        system_cpu = random.uniform(3.0, 10.0)
        iowait_cpu = random.uniform(0.2, 1.8)
        idle_cpu = 100.0 - (user_cpu + system_cpu + iowait_cpu)
        load1 = random.uniform(0.35, 1.20)
        load5 = random.uniform(0.40, 1.10)
        load15 = random.uniform(0.45, 0.95)
        csw_sec = random.uniform(1800.0, 4200.0)

        mem_total = 8192.0
        mem_used = random.uniform(2800.0, 4200.0)
        mem_avail = mem_total - mem_used
        mem_pct = (mem_used / mem_total) * 100.0
        swap_pct = random.uniform(0.0, 4.0)

        soc_temp = random.uniform(41.0, 48.0)
        thermal_zones = [soc_temp, soc_temp - 2.5, soc_temp - 4.0]
        throttled = 0
        cooling = False

        irq_sec = random.uniform(1500.0, 3800.0)
        procs_running = random.randint(1, 3)
        procs_blocked = 0
        zombies = 0

        disk_write = random.uniform(120.0, 850.0)
        net_rx_drops = 0
        uart_errors = 0

        # Anomaly mode perturbations
        if anomaly_mode == "thermal_throttling":
            soc_temp = random.uniform(86.5, 94.0)
            thermal_zones = [soc_temp, soc_temp - 1.5, soc_temp - 3.0]
            throttled = 3
            cooling = True
            user_cpu = random.uniform(65.0, 92.0)
            system_cpu = random.uniform(15.0, 25.0)
            load1 = random.uniform(4.5, 8.5)

        elif anomaly_mode == "memory_exhaustion":
            mem_used = random.uniform(7700.0, 8100.0)
            mem_pct = (mem_used / mem_total) * 100.0
            mem_avail = mem_total - mem_used
            swap_pct = random.uniform(82.0, 96.0)
            disk_write = random.uniform(18000.0, 45000.0)  # Swap thrashing
            procs_blocked = random.randint(3, 8)
            load1 = random.uniform(3.5, 6.0)

        elif anomaly_mode == "interrupt_storm":
            irq_sec = random.uniform(68000.0, 115000.0)
            system_cpu = random.uniform(70.0, 95.0)
            csw_sec = random.uniform(65000.0, 120000.0)
            procs_running = random.randint(6, 14)
            load1 = random.uniform(5.0, 11.0)

        elif anomaly_mode == "iowait_thrashing":
            iowait_cpu = random.uniform(58.0, 84.0)
            procs_blocked = random.randint(14, 28)
            disk_write = random.uniform(35000.0, 75000.0)
            load1 = random.uniform(6.5, 14.0)

        elif anomaly_mode == "network_packet_loss":
            net_rx_drops = random.randint(1200, 4800)
            system_cpu = random.uniform(25.0, 45.0)
            irq_sec = random.uniform(12000.0, 24000.0)

        return LinuxTelemetryPacket(
            timestamp=timestamp,
            hostname=self.hostname,
            cpu=LinuxCPUMetrics(
                user_pct=round(user_cpu, 1),
                system_pct=round(system_cpu, 1),
                iowait_pct=round(iowait_cpu, 1),
                idle_pct=round(max(0.0, idle_cpu), 1),
                loadavg_1m=round(load1, 2),
                loadavg_5m=round(load5, 2),
                loadavg_15m=round(load15, 2),
                context_switches_per_sec=round(csw_sec, 1),
            ),
            memory=LinuxMemoryMetrics(
                total_mb=round(mem_total, 1),
                available_mb=round(mem_avail, 1),
                used_mb=round(mem_used, 1),
                used_pct=round(mem_pct, 1),
                buffers_mb=180.0,
                cached_mb=1450.0,
                swap_total_mb=2048.0,
                swap_used_mb=round((swap_pct / 100.0) * 2048.0, 1),
                swap_used_pct=round(swap_pct, 1),
            ),
            thermal=LinuxThermalMetrics(
                soc_max_temp_c=round(soc_temp, 1),
                thermal_zones=[round(t, 1) for t in thermal_zones],
                cooling_active=cooling,
                throttled_state=throttled,
            ),
            kernel=LinuxKernelMetrics(
                interrupts_per_sec=round(irq_sec, 1),
                processes_running=procs_running,
                processes_blocked=procs_blocked,
                zombie_count=zombies,
            ),
            io=LinuxIOMetrics(
                disk_read_kb_per_sec=round(random.uniform(50.0, 400.0), 1),
                disk_write_kb_per_sec=round(disk_write, 1),
                network_rx_kb_per_sec=round(random.uniform(20.0, 300.0), 1),
                network_tx_kb_per_sec=round(random.uniform(10.0, 180.0), 1),
                network_rx_drop_count=net_rx_drops,
                network_tx_drop_count=0,
            ),
            buses=LinuxBusMetrics(
                usb_device_count=3,
                i2c_bus_count=2,
                spi_devices_active=1,
                uart_error_count=uart_errors,
            ),
        )
