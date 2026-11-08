#!/usr/bin/env python3
"""
Mock Bluetooth Client & Telemetry Monitor
Interfaces with Smart Home Automation via Serial COM Port (HC-05 or USB Serial).
"""

import argparse
import json
import sys
import time

try:
    import serial
except ImportError:
    serial = None


def run_interactive(port: str, baud: int = 9600):
    if serial is None:
        print("[ERR] pyserial not installed. Run: pip install pyserial")
        sys.exit(1)

    print(f"[*] Opening serial link on {port} @ {baud} baud...")
    try:
        ser = serial.Serial(port, baud, timeout=1.0)
        time.sleep(2.0)  # Wait for Arduino reset
        print("[+] Link established! Available commands:")
        print("    L1_ON, L1_OFF, L2_ON, L2_OFF")
        print("    M_ON, M_OFF, M_SPEED:<0-255>")
        print("    SERVO:<0-180>")
        print("    PUMP_ON, PUMP_OFF, MODE_AUTO, MODE_MANUAL")
        print("    SET_TH:<0-100>, STATUS?")
        print("    Type 'exit' or 'quit' to terminate.")

        while True:
            cmd = input("\n[SmartHome]> ").strip()
            if cmd.lower() in ("exit", "quit"):
                break
            if not cmd:
                continue

            ser.write((cmd + "\n").encode("utf-8"))
            time.sleep(0.1)

            while ser.in_waiting:
                line = ser.readline().decode("utf-8", errors="replace").strip()
                if line:
                    print(f"  [DEVICE RESPONSE] {line}")

        ser.close()
        print("[*] Connection closed.")
    except Exception as e:
        print(f"[ERR] Failed to communicate on {port}: {e}")


def run_batch_test(port: str, baud: int = 9600):
    if serial is None:
        print("[ERR] pyserial required for batch test.")
        sys.exit(1)

    test_commands = [
        "STATUS?",
        "L1_ON",
        "L2_ON",
        "M_SPEED:150",
        "SERVO:45",
        "MODE_AUTO",
        "SET_TH:40",
        "STATUS?"
    ]

    print(f"[*] Executing automated verification batch on {port}...")
    ser = serial.Serial(port, baud, timeout=1.0)
    time.sleep(2.0)

    for cmd in test_commands:
        print(f"\n[TX] -> {cmd}")
        ser.write((cmd + "\n").encode("utf-8"))
        time.sleep(0.3)
        while ser.in_waiting:
            line = ser.readline().decode("utf-8", errors="replace").strip()
            print(f"[RX] <- {line}")

    ser.close()
    print("\n[+] Batch execution completed.")


def main():
    parser = argparse.ArgumentParser(description="Smart Home Bluetooth Telemetry Client")
    parser.add_argument("--port", type=str, default="COM3", help="Serial port (e.g., COM3, /dev/ttyUSB0)")
    parser.add_argument("--baud", type=int, default=9600, help="Baud rate (default 9600)")
    parser.add_argument("--batch", action="store_true", help="Run automated command sequence")
    args = parser.parse_args()

    if args.batch:
        run_batch_test(args.port, args.baud)
    else:
        run_interactive(args.port, args.baud)


if __name__ == "__main__":
    main()
