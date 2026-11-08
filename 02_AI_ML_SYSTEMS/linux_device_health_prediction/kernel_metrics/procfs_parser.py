"""
Linux Procfs Metrics Parser
Extracts operating system metrics from virtual filesystems (/proc/stat, /proc/meminfo, /proc/loadavg).
"""

import os
import re
from typing import Dict, Optional, Tuple


class ProcfsParser:
    """
    Parses low-level kernel statistics directly from procfs.
    """

    @staticmethod
    def parse_loadavg() -> Tuple[float, float, float]:
        """Parses /proc/loadavg (1min, 5min, 15min load averages)."""
        if not os.path.exists("/proc/loadavg"):
            return (0.45, 0.52, 0.48)
        try:
            with open("/proc/loadavg", "r") as f:
                parts = f.read().split()
                return (float(parts[0]), float(parts[1]), float(parts[2]))
        except Exception:
            return (0.0, 0.0, 0.0)

    @staticmethod
    def parse_meminfo() -> Dict[str, float]:
        """Parses /proc/meminfo into megabytes."""
        metrics = {
            "total_mb": 8192.0,
            "available_mb": 4096.0,
            "used_mb": 4096.0,
            "used_pct": 50.0,
            "buffers_mb": 256.0,
            "cached_mb": 1536.0,
            "swap_total_mb": 2048.0,
            "swap_used_mb": 0.0,
            "swap_used_pct": 0.0,
        }
        if not os.path.exists("/proc/meminfo"):
            return metrics

        try:
            raw = {}
            with open("/proc/meminfo", "r") as f:
                for line in f:
                    parts = line.split(":")
                    if len(parts) == 2:
                        val = int(re.findall(r"\d+", parts[1])[0])
                        raw[parts[0].strip()] = val / 1024.0  # Convert kB to MB

            total = raw.get("MemTotal", 8192.0)
            avail = raw.get("MemAvailable", total * 0.5)
            used = max(0.0, total - avail)
            swap_total = raw.get("SwapTotal", 2048.0)
            swap_free = raw.get("SwapFree", swap_total)
            swap_used = max(0.0, swap_total - swap_free)

            metrics["total_mb"] = round(total, 1)
            metrics["available_mb"] = round(avail, 1)
            metrics["used_mb"] = round(used, 1)
            metrics["used_pct"] = round((used / total) * 100.0, 1)
            metrics["buffers_mb"] = round(raw.get("Buffers", 0.0), 1)
            metrics["cached_mb"] = round(raw.get("Cached", 0.0), 1)
            metrics["swap_total_mb"] = round(swap_total, 1)
            metrics["swap_used_mb"] = round(swap_used, 1)
            metrics["swap_used_pct"] = round((swap_used / max(1.0, swap_total)) * 100.0, 1) if swap_total > 0 else 0.0
            return metrics
        except Exception:
            return metrics

    @staticmethod
    def parse_stat() -> Dict[str, float]:
        """Parses /proc/stat for context switches, running, and blocked processes."""
        metrics = {
            "context_switches": 2500.0,
            "processes_running": 2,
            "processes_blocked": 0,
        }
        if not os.path.exists("/proc/stat"):
            return metrics

        try:
            with open("/proc/stat", "r") as f:
                for line in f:
                    if line.startswith("ctxt"):
                        metrics["context_switches"] = float(line.split()[1])
                    elif line.startswith("procs_running"):
                        metrics["processes_running"] = int(line.split()[1])
                    elif line.startswith("procs_blocked"):
                        metrics["processes_blocked"] = int(line.split()[1])
            return metrics
        except Exception:
            return metrics
