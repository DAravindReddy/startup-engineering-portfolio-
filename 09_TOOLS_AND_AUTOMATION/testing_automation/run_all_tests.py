#!/usr/bin/env python3
"""
Kernelwise Labs - Monorepo Test Runner & Quality Gate Orchestrator
Cross-platform automated testing suite for C, C++, and Python components.
"""

import os
import sys
import subprocess
import time

GREEN = "\033[92m"
RED = "\033[91m"
CYAN = "\033[96m"
RESET = "\033[0m"

MONOREPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))

TEST_DEFINITIONS = [
    {
        "name": "Shared C Libraries (i2c, uart, dht11, packet)",
        "cmd": [
            "gcc", "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11", "-c",
            "08_SHARED_LIBRARIES/common_embedded_drivers/i2c_hal.c",
            "08_SHARED_LIBRARIES/common_embedded_drivers/uart_hal.c",
            "08_SHARED_LIBRARIES/sensor_libraries/dht11_driver.c",
            "08_SHARED_LIBRARIES/communication_protocols/packet_protocol.c",
            "-I", "08_SHARED_LIBRARIES/common_embedded_drivers",
            "-I", "08_SHARED_LIBRARIES/sensor_libraries",
            "-I", "08_SHARED_LIBRARIES/communication_protocols"
        ],
        "cleanup": ["i2c_hal.o", "uart_hal.o", "dht11_driver.o", "packet_protocol.o"]
    },
    {
        "name": "Smart Home Automation Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/include",
            "01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/actuator_controller.cpp",
            "01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/bluetooth_handler.cpp",
            "01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/irrigation_system.cpp",
            "01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/sensor_manager.cpp",
            "01_EMBEDDED_SYSTEMS/smart_home_automation/tests/unit_tests/test_smart_home.cpp",
            "-o", "test_smart_home.tmp"
        ],
        "cleanup": ["test_smart_home.tmp", "test_smart_home.tmp.exe"]
    },
    {
        "name": "Object Avoiding Robot Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/include",
            "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/motor_driver.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/navigation_fsm.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/servo_scanner.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/ultrasonic_sensor.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/tests/unit_tests/test_navigation_fsm.cpp",
            "-o", "test_nav.tmp"
        ],
        "cleanup": ["test_nav.tmp", "test_nav.tmp.exe"]
    },
    {
        "name": "Fire Fighting Robot Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/include",
            "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/fire_fighting_fsm.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/flame_sensor_array.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/motor_driver.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/pump_controller.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/tests/unit_tests/test_fire_fighting_fsm.cpp",
            "-o", "test_ff.tmp"
        ],
        "cleanup": ["test_ff.tmp", "test_ff.tmp.exe"]
    },
    {
        "name": "Gesture Controlled Robotic Arm Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/include",
            "01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/kinematics_mapper.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/mpu6050_imu.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/servo_controller.cpp",
            "01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/tests/unit_tests/test_kinematics_mapper.cpp",
            "-o", "test_arm.tmp"
        ],
        "cleanup": ["test_arm.tmp", "test_arm.tmp.exe"]
    },
    {
        "name": "Smart Railway Track System Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/include",
            "01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/crack_detector.cpp",
            "01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/obstacle_sonar.cpp",
            "01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/railway_fsm.cpp",
            "01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/signal_gate_controller.cpp",
            "01_EMBEDDED_SYSTEMS/smart_railway_system/tests/unit_tests/test_railway_fsm.cpp",
            "-o", "test_rail.tmp"
        ],
        "cleanup": ["test_rail.tmp", "test_rail.tmp.exe"]
    },
    {
        "name": "TinyML Arduino BLE Sense AI Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/include",
            "01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/src/tinyml_anomaly_detector.cpp",
            "01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/src/feature_extractor.cpp",
            "01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/tests/unit_tests/test_tinyml_pipeline.cpp",
            "-o", "test_tinyml.tmp"
        ],
        "cleanup": ["test_tinyml.tmp", "test_tinyml.tmp.exe"]
    },
    {
        "name": "Smart Garden Irrigation Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "03_IOT_AND_AUTOMATION/smart_garden_irrigation/source_code/include",
            "03_IOT_AND_AUTOMATION/smart_garden_irrigation/source_code/src/irrigation_controller.c",
            "03_IOT_AND_AUTOMATION/smart_garden_irrigation/tests/test_irrigation_controller.cpp",
            "-o", "test_garden.tmp"
        ],
        "cleanup": ["test_garden.tmp", "test_garden.tmp.exe"]
    },
    {
        "name": "Multi-Sensor Monitoring Test Suite (C++)",
        "cmd": [
            "g++", "-std=c++14", "-Wall", "-Wextra", "-Werror",
            "-I", "03_IOT_AND_AUTOMATION/sensor_monitoring/source_code/include",
            "03_IOT_AND_AUTOMATION/sensor_monitoring/source_code/src/sensor_monitor.c",
            "03_IOT_AND_AUTOMATION/sensor_monitoring/tests/test_sensor_monitoring.cpp",
            "-o", "test_sensor.tmp"
        ],
        "cleanup": ["test_sensor.tmp", "test_sensor.tmp.exe"]
    },
    {
        "name": "Embedded Linux Digital Watch Test Suite (C11)",
        "cmd": [
            "gcc", "-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic",
            "-I", "05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/include",
            "05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/src/watch_core.c",
            "05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/tests/test_watch_core.c",
            "-o", "test_watch.tmp"
        ],
        "cleanup": ["test_watch.tmp", "test_watch.tmp.exe"]
    },
    {
        "name": "Modern C++17 Core Suite",
        "cmd": [
            "g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
            "-I", "04_SOFTWARE_ENGINEERING/CPP_programming/include",
            "04_SOFTWARE_ENGINEERING/CPP_programming/src/main.cpp",
            "-o", "test_cpp.tmp"
        ],
        "cleanup": ["test_cpp.tmp", "test_cpp.tmp.exe"]
    },
    {
        "name": "Android Device Health Prediction (Python unittests)",
        "cmd": [
            sys.executable, "-m", "unittest", "discover",
            "-s", "02_AI_ML_SYSTEMS/android_device_health_prediction/tests",
            "-p", "test_*.py"
        ],
        "cleanup": []
    },
    {
        "name": "Linux Device Health Prediction (Python unittests)",
        "cmd": [
            sys.executable, "-m", "unittest", "discover",
            "-s", "02_AI_ML_SYSTEMS/linux_device_health_prediction/tests",
            "-p", "test_*.py"
        ],
        "cleanup": []
    }
]

def main():
    print(f"{CYAN}====================================================================={RESET}")
    print(f"{CYAN} Kernelwise Labs: Monorepo Multi-Language Test Runner                {RESET}")
    print(f"{CYAN} Root: {MONOREPO_ROOT}{RESET}")
    print(f"{CYAN}====================================================================={RESET}")

    passed = 0
    failed = 0
    start_time = time.time()

    for test in TEST_DEFINITIONS:
        name = test["name"]
        print(f"[RUNNING] {name:<60} ... ", end="", flush=True)

        res = subprocess.run(
            test["cmd"],
            cwd=MONOREPO_ROOT,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        for c_file in test.get("cleanup", []):
            p = os.path.join(MONOREPO_ROOT, c_file)
            if os.path.exists(p):
                try: os.remove(p)
                except Exception: pass

        if res.returncode == 0:
            print(f"{GREEN}PASS{RESET}")
            passed += 1
        else:
            print(f"{RED}FAIL{RESET}")
            print(res.stderr)
            failed += 1

    total_time = time.time() - start_time
    print(f"{CYAN}====================================================================={RESET}")
    status_color = GREEN if failed == 0 else RED
    print(f"{status_color}SUMMARY: {passed} Passed, {failed} Failed (Time: {total_time:.2f}s){RESET}")
    print(f"{CYAN}====================================================================={RESET}")

    sys.exit(0 if failed == 0 else 1)

if __name__ == "__main__":
    main()
