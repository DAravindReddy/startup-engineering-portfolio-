"""
Android ADB Telemetry Collector
Interfaces with live physical Android devices over ADB shell dumpsys
and includes a realistic synthetic telemetry emulator with anomaly injection modes.
"""

import math
import random
import re
import subprocess
import time
from typing import Dict, List, Optional
from telemetry.schema import (
    AndroidTelemetryPacket,
    CPUMetrics,
    GPUMetrics,
    ThermalMetrics,
    MemoryMetrics,
    SubsystemMetrics,
)


class ADBDeviceCollector:
    """
    Acquires real-time telemetry from physical Android devices via ADB
    or runs synthetic simulation if hardware is not connected.
    """

    def __init__(self, device_id: Optional[str] = None, force_mock: bool = False):
        self.device_id = device_id
        self.force_mock = force_mock
        self._sim_step = 0
        self.is_live = False

        if not self.force_mock:
            self.is_live = self._check_adb_connection()

        if not self.is_live:
            self.device_id = self.device_id or "sim-android-arm64"

    def _check_adb_connection(self) -> bool:
        try:
            res = subprocess.run(["adb", "devices"], capture_output=True, text=True, timeout=2)
            lines = [l.strip() for l in res.stdout.splitlines() if l.strip() and not l.startswith("List")]
            if lines:
                if not self.device_id:
                    self.device_id = lines[0].split("\t")[0]
                return True
            return False
        except Exception:
            return False

    def collect(self, inject_anomaly: Optional[str] = None) -> AndroidTelemetryPacket:
        """
        Samples a snapshot of device telemetry.
        
        Args:
            inject_anomaly: None, 'thermal', 'memory_leak', 'cpu_hog', or 'subsystem_stall'
        """
        if self.is_live:
            return self._collect_live_adb()
        return self._collect_synthetic(inject_anomaly)

    def _collect_synthetic(self, anomaly_type: Optional[str] = None) -> AndroidTelemetryPacket:
        self._sim_step += 1
        now = time.time()
        wave = math.sin(self._sim_step / 12.0)

        # Baseline normal dynamics
        cpu_base = max(10.0, min(95.0, 42.0 + (wave * 25.0) + random.uniform(-3, 3)))
        gpu_base = max(5.0, min(90.0, 25.0 + (wave * 20.0) + random.uniform(-2, 4)))
        ram_used_pct = max(35.0, min(98.0, 58.0 + (wave * 12.0) + random.uniform(-1, 2)))
        soc_temp = 38.0 + (cpu_base * 0.25) + random.uniform(-0.5, 0.5)
        batt_temp = 31.0 + (soc_temp * 0.28) + random.uniform(-0.2, 0.2)
        throttling_level = 0
        lmk_kills = 0
        bt_drops = 0
        cam_drops = 0
        media_stalls = 0

        # Anomaly Injection Modes
        if anomaly_type == "thermal":
            soc_temp = random.uniform(85.0, 94.0)
            batt_temp = random.uniform(48.0, 55.0)
            throttling_level = 3  # Severe throttling
            cpu_base = max(cpu_base, 88.0)
        elif anomaly_type == "memory_leak":
            ram_used_pct = random.uniform(96.0, 99.5)
            lmk_kills = random.randint(3, 8)
        elif anomaly_type == "cpu_hog":
            cpu_base = random.uniform(94.0, 99.5)
            soc_temp = random.uniform(78.0, 86.0)
            throttling_level = 2
        elif anomaly_type == "subsystem_stall":
            bt_drops = random.randint(4, 12)
            cam_drops = random.randint(15, 45)
            media_stalls = random.randint(5, 15)

        cpu_user = round(cpu_base * 0.75, 1)
        cpu_kernel = round(cpu_base - cpu_user, 1)

        total_ram = 4096.0  # 4 GB RAM
        used_mb = round((ram_used_pct / 100.0) * total_ram, 1)
        free_mb = round(total_ram - used_mb, 1)

        top_procs = [
            {"name": "com.android.systemui", "cpu_pct": round(cpu_user * 0.35, 1)},
            {"name": "system_server", "cpu_pct": round(cpu_kernel * 0.8, 1)},
            {"name": "com.android.chrome", "cpu_pct": round(cpu_user * 0.45, 1)},
            {"name": "surfaceflinger", "cpu_pct": round(random.uniform(4.0, 10.0), 1)},
        ]

        return AndroidTelemetryPacket(
            timestamp=now,
            device_id=self.device_id,
            cpu=CPUMetrics(
                user_pct=cpu_user,
                kernel_pct=cpu_kernel,
                total_pct=round(cpu_base, 1),
                frequency_mhz=1800.0 if throttling_level == 0 else 1200.0,
                top_processes=top_procs,
            ),
            gpu=GPUMetrics(
                utilization_pct=round(gpu_base, 1),
                frequency_mhz=650.0,
                frame_drop_count=cam_drops // 2,
            ),
            thermal=ThermalMetrics(
                soc_temp_c=round(soc_temp, 1),
                battery_temp_c=round(batt_temp, 1),
                throttling_level=throttling_level,
            ),
            memory=MemoryMetrics(
                total_mb=total_ram,
                used_mb=used_mb,
                free_mb=free_mb,
                used_pct=round(ram_used_pct, 1),
                lmk_kill_count=lmk_kills,
            ),
            subsystems=SubsystemMetrics(
                bluetooth_connected=True,
                bluetooth_drop_count=bt_drops,
                camera_frames_dropped=cam_drops,
                camera_active=False,
                media_codec_stalls=media_stalls,
            ),
        )

    def _collect_live_adb(self) -> AndroidTelemetryPacket:
        now = time.time()
        # Battery dumpsys
        res_batt = subprocess.run(
            ["adb", "-s", self.device_id, "shell", "dumpsys", "battery"],
            capture_output=True, text=True, timeout=3
        )
        batt_temp = 32.0
        for line in res_batt.stdout.splitlines():
            if "temperature:" in line:
                m = re.search(r"temperature:\s*(\d+)", line)
                if m: batt_temp = float(m.group(1)) / 10.0

        # Memory dumpsys
        res_mem = subprocess.run(
            ["adb", "-s", self.device_id, "shell", "dumpsys", "meminfo"],
            capture_output=True, text=True, timeout=3
        )
        total_ram = 4096.0
        used_ram = 2048.0
        for line in res_mem.stdout.splitlines():
            if "Total RAM:" in line:
                m = re.findall(r"[\d,]+", line)
                if m: total_ram = float(m[0].replace(",", "")) / 1024.0
            if "Used RAM:" in line:
                m = re.findall(r"[\d,]+", line)
                if m: used_ram = float(m[0].replace(",", "")) / 1024.0

        ram_pct = round((used_ram / total_ram) * 100.0, 1)

        # CPU dumpsys
        res_cpu = subprocess.run(
            ["adb", "-s", self.device_id, "shell", "dumpsys", "cpuinfo"],
            capture_output=True, text=True, timeout=3
        )
        cpu_total = 25.0
        for line in res_cpu.stdout.splitlines():
            if "TOTAL:" in line and "%" in line:
                m = re.search(r"(\d+(?:\.\d+)?)%\s*TOTAL", line)
                if m: cpu_total = float(m.group(1))

        return AndroidTelemetryPacket(
            timestamp=now,
            device_id=self.device_id,
            cpu=CPUMetrics(
                user_pct=round(cpu_total * 0.7, 1),
                kernel_pct=round(cpu_total * 0.3, 1),
                total_pct=round(cpu_total, 1),
                frequency_mhz=1800.0,
                top_processes=[],
            ),
            gpu=GPUMetrics(utilization_pct=15.0, frequency_mhz=500.0, frame_drop_count=0),
            thermal=ThermalMetrics(soc_temp_c=batt_temp + 6.0, battery_temp_c=batt_temp, throttling_level=0),
            memory=MemoryMetrics(total_mb=total_ram, used_mb=used_ram, free_mb=total_ram - used_ram, used_pct=ram_pct, lmk_kill_count=0),
            subsystems=SubsystemMetrics(bluetooth_connected=True, bluetooth_drop_count=0, camera_frames_dropped=0, camera_active=False, media_codec_stalls=0),
        )
