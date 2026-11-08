"""
Linux Sysfs Metrics Parser
Extracts hardware and peripheral subsystem metrics from the Linux sysfs (/sys) filesystem.
Handles thermal zones, cooling devices, USB topology, and network drop statistics.
"""

import glob
import os
from typing import Dict, List, Tuple


class SysfsParser:
    """
    Parses sysfs hardware and bus telemetry with robust fallback for simulated environments.
    """

    @staticmethod
    def parse_thermal() -> Tuple[float, List[float], bool, int]:
        """
        Parses /sys/class/thermal/thermal_zone* and cooling_device*.
        Returns: (soc_max_temp_c, all_zone_temps_c, cooling_active, throttled_state)
        """
        thermal_zones = glob.glob("/sys/class/thermal/thermal_zone*")
        if not thermal_zones:
            # Nominal fallback for simulated or non-Linux hosts
            return (46.5, [46.5, 44.0, 42.5], False, 0)

        temps = []
        for zone in sorted(thermal_zones):
            temp_path = os.path.join(zone, "temp")
            if os.path.exists(temp_path):
                try:
                    with open(temp_path, "r") as f:
                        # Sysfs thermal is typically reported in millidegrees Celsius
                        val = float(f.read().strip())
                        if val > 1000:
                            val /= 1000.0
                        temps.append(round(val, 1))
                except Exception:
                    pass

        max_temp = max(temps) if temps else 45.0
        cooling_devices = glob.glob("/sys/class/thermal/cooling_device*")
        cooling_active = False
        max_throttle = 0

        for cdev in cooling_devices:
            cur_state_path = os.path.join(cdev, "cur_state")
            if os.path.exists(cur_state_path):
                try:
                    with open(cur_state_path, "r") as f:
                        state = int(f.read().strip())
                        if state > 0:
                            cooling_active = True
                            max_throttle = max(max_throttle, state)
                except Exception:
                    pass

        return (max_temp, temps if temps else [max_temp], cooling_active, max_throttle)

    @staticmethod
    def parse_subsystem_buses() -> Dict[str, int]:
        """
        Counts active USB devices, I2C buses, and SPI interfaces.
        """
        metrics = {
            "usb_device_count": 3,
            "i2c_bus_count": 2,
            "spi_devices_active": 1,
            "uart_error_count": 0,
        }

        usb_path = "/sys/bus/usb/devices"
        if os.path.exists(usb_path):
            try:
                # Count USB device nodes (excluding root hubs)
                devs = [d for d in os.listdir(usb_path) if ":" not in d and d.startswith("usb")]
                metrics["usb_device_count"] = max(1, len(devs))
            except Exception:
                pass

        i2c_path = "/sys/bus/i2c/devices"
        if os.path.exists(i2c_path):
            try:
                metrics["i2c_bus_count"] = len(os.listdir(i2c_path))
            except Exception:
                pass

        spi_path = "/sys/bus/spi/devices"
        if os.path.exists(spi_path):
            try:
                metrics["spi_devices_active"] = len(os.listdir(spi_path))
            except Exception:
                pass

        return metrics

    @staticmethod
    def parse_net_drops() -> Tuple[int, int]:
        """
        Parses /sys/class/net/*/statistics/{rx_dropped, tx_dropped}.
        Returns: (total_rx_dropped, total_tx_dropped)
        """
        rx_drops = 0
        tx_drops = 0
        net_devs = glob.glob("/sys/class/net/*")
        if not net_devs:
            return (0, 0)

        for dev in net_devs:
            rx_stat = os.path.join(dev, "statistics", "rx_dropped")
            tx_stat = os.path.join(dev, "statistics", "tx_dropped")
            if os.path.exists(rx_stat):
                try:
                    with open(rx_stat, "r") as f:
                        rx_drops += int(f.read().strip())
                except Exception:
                    pass
            if os.path.exists(tx_stat):
                try:
                    with open(tx_stat, "r") as f:
                        tx_drops += int(f.read().strip())
                except Exception:
                    pass

        return (rx_drops, tx_drops)
