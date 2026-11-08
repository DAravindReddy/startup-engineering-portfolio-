#!/usr/bin/env bash
# ==============================================================================
# Kernelwise Labs - Repository Integrity & Compilation Validation Script (Bash)
# ==============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MONOREPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
cd "${MONOREPO_ROOT}"

echo "====================================================================="
echo " Kernelwise Labs: Monorepo Compilation & Integrity Validation (Bash) "
echo " Monorepo Root: ${MONOREPO_ROOT}"
echo "====================================================================="

PASSED=0
FAILED=0

run_check() {
    local name="$1"
    shift
    echo -n "[CHECKING] ${name} ... "
    if "$@" >/dev/null 2>&1; then
        echo "PASS"
        ((PASSED++))
    else
        echo "FAIL"
        ((FAILED++))
    fi
}

# 1. Shared Libraries
run_check "Shared C Libraries" gcc -Wall -Wextra -Werror -pedantic -std=c11 -c \
    08_SHARED_LIBRARIES/common_embedded_drivers/i2c_hal.c \
    08_SHARED_LIBRARIES/common_embedded_drivers/uart_hal.c \
    08_SHARED_LIBRARIES/sensor_libraries/dht11_driver.c \
    08_SHARED_LIBRARIES/communication_protocols/packet_protocol.c \
    -I 08_SHARED_LIBRARIES/common_embedded_drivers \
    -I 08_SHARED_LIBRARIES/sensor_libraries \
    -I 08_SHARED_LIBRARIES/communication_protocols
rm -f *.o

# 2. Smart Home Automation
run_check "Smart Home Automation" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/include \
    01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/actuator_controller.cpp \
    01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/bluetooth_handler.cpp \
    01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/irrigation_system.cpp \
    01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/sensor_manager.cpp \
    01_EMBEDDED_SYSTEMS/smart_home_automation/tests/unit_tests/test_smart_home.cpp -o /dev/null

# 3. Object Avoiding Robot
run_check "Object Avoiding Robot" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/include \
    01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/motor_driver.cpp \
    01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/navigation_fsm.cpp \
    01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/servo_scanner.cpp \
    01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/ultrasonic_sensor.cpp \
    01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/tests/unit_tests/test_navigation_fsm.cpp -o /dev/null

# 4. Fire Fighting Robot
run_check "Fire Fighting Robot" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/include \
    01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/fire_fighting_fsm.cpp \
    01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/flame_sensor_array.cpp \
    01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/motor_driver.cpp \
    01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/pump_controller.cpp \
    01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/tests/unit_tests/test_fire_fighting_fsm.cpp -o /dev/null

# 5. Gesture Controlled Robotic Arm
run_check "Gesture Controlled Arm" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/include \
    01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/kinematics_mapper.cpp \
    01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/mpu6050_imu.cpp \
    01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/servo_controller.cpp \
    01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/tests/unit_tests/test_kinematics_mapper.cpp -o /dev/null

# 6. Smart Railway System
run_check "Smart Railway Track" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/include \
    01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/crack_detector.cpp \
    01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/obstacle_sonar.cpp \
    01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/railway_fsm.cpp \
    01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/signal_gate_controller.cpp \
    01_EMBEDDED_SYSTEMS/smart_railway_system/tests/unit_tests/test_railway_fsm.cpp -o /dev/null

# 7. Arduino BLE Sense TinyML
run_check "TinyML BLE Sense AI" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/include \
    01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/src/tinyml_anomaly_detector.cpp \
    01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/src/feature_extractor.cpp \
    01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/tests/unit_tests/test_tinyml_pipeline.cpp -o /dev/null

# 8. Smart Garden Irrigation
run_check "Smart Garden Irrigation" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 03_IOT_AND_AUTOMATION/smart_garden_irrigation/source_code/include \
    03_IOT_AND_AUTOMATION/smart_garden_irrigation/source_code/src/irrigation_controller.c \
    03_IOT_AND_AUTOMATION/smart_garden_irrigation/tests/test_irrigation_controller.cpp -o /dev/null

# 9. Sensor Monitoring
run_check "Sensor Monitoring" g++ -std=c++14 -Wall -Wextra -Werror \
    -I 03_IOT_AND_AUTOMATION/sensor_monitoring/source_code/include \
    03_IOT_AND_AUTOMATION/sensor_monitoring/source_code/src/sensor_monitor.c \
    03_IOT_AND_AUTOMATION/sensor_monitoring/tests/test_sensor_monitoring.cpp -o /dev/null

# 10. Digital Watch System
run_check "Digital Watch System" gcc -std=c11 -Wall -Wextra -Werror -pedantic \
    -I 05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/include \
    05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/src/watch_core.c \
    05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/tests/test_watch_core.c -o /dev/null

# 11. Modern C++17 Core
run_check "Modern C++17 Core" g++ -std=c++17 -Wall -Wextra -Werror \
    -I 04_SOFTWARE_ENGINEERING/CPP_programming/include \
    04_SOFTWARE_ENGINEERING/CPP_programming/src/main.cpp -o /dev/null -pthread

# 12. Python Unittests
run_check "Android AI Unittests" python3 -m unittest discover -s 02_AI_ML_SYSTEMS/android_device_health_prediction/tests -p "test_*.py"
run_check "Linux AI Unittests" python3 -m unittest discover -s 02_AI_ML_SYSTEMS/linux_device_health_prediction/tests -p "test_*.py"

echo "====================================================================="
echo " VALIDATION SUMMARY: ${PASSED} Passed, ${FAILED} Failed"
echo "====================================================================="

if [ "${FAILED}" -ne 0 ]; then
    exit 1
fi
exit 0
