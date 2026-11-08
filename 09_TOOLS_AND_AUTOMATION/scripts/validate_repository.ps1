# ==============================================================================
# Kernelwise Labs - Repository Integrity & Compilation Validation Script
# ==============================================================================
param(
    [switch]$SkipPython = $false
)

$MonorepoRoot = Resolve-Path "$PSScriptRoot\..\.."
Set-Location $MonorepoRoot

Write-Host "=====================================================================" -ForegroundColor Cyan
Write-Host " Kernelwise Labs: Monorepo Compilation & Integrity Validation        " -ForegroundColor Cyan
Write-Host " Monorepo Root: $MonorepoRoot" -ForegroundColor Cyan
Write-Host "=====================================================================" -ForegroundColor Cyan

$PassedCount = 0
$FailedCount = 0

function Run-BuildCheck {
    param(
        [string]$Name,
        [string]$Command
    )
    Write-Host -NoNewline "[CHECKING] $Name ... "
    $null = & cmd.exe /c "$Command >nul 2>&1"
    if ($LASTEXITCODE -eq 0) {
        Write-Host "PASS" -ForegroundColor Green
        $script:PassedCount++
    } else {
        Write-Host "FAIL" -ForegroundColor Red
        $script:FailedCount++
    }
}

# 1. Shared Libraries Compilation
Run-BuildCheck "Shared C Libraries (i2c_hal, uart_hal, dht11, packet_protocol)" `
    "gcc -Wall -Wextra -Werror -pedantic -std=c11 -c 08_SHARED_LIBRARIES/common_embedded_drivers/i2c_hal.c 08_SHARED_LIBRARIES/common_embedded_drivers/uart_hal.c 08_SHARED_LIBRARIES/sensor_libraries/dht11_driver.c 08_SHARED_LIBRARIES/communication_protocols/packet_protocol.c -I 08_SHARED_LIBRARIES/common_embedded_drivers -I 08_SHARED_LIBRARIES/sensor_libraries -I 08_SHARED_LIBRARIES/communication_protocols && del /f /q *.o"

# 2. Smart Home Automation Unit Tests
Run-BuildCheck "Smart Home Automation Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/include 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/actuator_controller.cpp 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/bluetooth_handler.cpp 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/irrigation_system.cpp 01_EMBEDDED_SYSTEMS/smart_home_automation/source_code/src/sensor_manager.cpp 01_EMBEDDED_SYSTEMS/smart_home_automation/tests/unit_tests/test_smart_home.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 3. Object Avoiding Robot Unit Tests
Run-BuildCheck "Object Avoiding Robot Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/include 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/motor_driver.cpp 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/navigation_fsm.cpp 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/servo_scanner.cpp 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/source_code/src/ultrasonic_sensor.cpp 01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot/tests/unit_tests/test_navigation_fsm.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 4. Fire Fighting Robot Unit Tests
Run-BuildCheck "Fire Fighting Robot Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/include 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/fire_fighting_fsm.cpp 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/flame_sensor_array.cpp 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/motor_driver.cpp 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/source_code/src/pump_controller.cpp 01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot/tests/unit_tests/test_fire_fighting_fsm.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 5. Gesture Controlled Robotic Arm Unit Tests
Run-BuildCheck "Gesture Controlled Arm Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/include 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/kinematics_mapper.cpp 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/mpu6050_imu.cpp 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/source_code/src/servo_controller.cpp 01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm/tests/unit_tests/test_kinematics_mapper.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 6. Smart Railway Track System Unit Tests
Run-BuildCheck "Smart Railway Track Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/include 01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/crack_detector.cpp 01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/obstacle_sonar.cpp 01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/railway_fsm.cpp 01_EMBEDDED_SYSTEMS/smart_railway_system/source_code/src/signal_gate_controller.cpp 01_EMBEDDED_SYSTEMS/smart_railway_system/tests/unit_tests/test_railway_fsm.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 7. Arduino BLE Sense TinyML Unit Tests
Run-BuildCheck "TinyML Arduino BLE Sense AI Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/include 01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/src/tinyml_anomaly_detector.cpp 01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/source_code/src/feature_extractor.cpp 01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai/tests/unit_tests/test_tinyml_pipeline.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 8. Smart Garden Irrigation Unit Tests
Run-BuildCheck "Smart Garden Irrigation Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 03_IOT_AND_AUTOMATION/smart_garden_irrigation/source_code/include 03_IOT_AND_AUTOMATION/smart_garden_irrigation/source_code/src/irrigation_controller.c 03_IOT_AND_AUTOMATION/smart_garden_irrigation/tests/test_irrigation_controller.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 9. Sensor Monitoring Unit Tests
Run-BuildCheck "Multi-Sensor Monitoring Test Suite (C++)" `
    "g++ -std=c++14 -Wall -Wextra -Werror -I 03_IOT_AND_AUTOMATION/sensor_monitoring/source_code/include 03_IOT_AND_AUTOMATION/sensor_monitoring/source_code/src/sensor_monitor.c 03_IOT_AND_AUTOMATION/sensor_monitoring/tests/test_sensor_monitoring.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 10. Embedded Linux Digital Watch Unit Tests
Run-BuildCheck "Embedded Linux Digital Watch Test Suite (C11)" `
    "gcc -std=c11 -Wall -Wextra -Werror -pedantic -I 05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/include 05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/src/watch_core.c 05_LINUX_AND_SYSTEMS/embedded_linux/digital_watch_system/tests/test_watch_core.c -o test_tmp.exe && del /f /q test_tmp.exe"

# 11. Modern C++17 Core Suite
Run-BuildCheck "Modern C++17 Core Suite" `
    "g++ -std=c++17 -Wall -Wextra -Werror -I 04_SOFTWARE_ENGINEERING/CPP_programming/include 04_SOFTWARE_ENGINEERING/CPP_programming/src/main.cpp -o test_tmp.exe && del /f /q test_tmp.exe"

# 12. Python Tests
if (-not $SkipPython) {
    Run-BuildCheck "Android AI Anomaly Detection Unit Tests (Python)" `
        "python -m unittest discover -s 02_AI_ML_SYSTEMS/android_device_health_prediction/tests -p test_*.py"
    Run-BuildCheck "Linux AI Anomaly Detection Unit Tests (Python)" `
        "python -m unittest discover -s 02_AI_ML_SYSTEMS/linux_device_health_prediction/tests -p test_*.py"
}

$SummaryColor = if ($FailedCount -eq 0) { "Green" } else { "Red" }
Write-Host "=====================================================================" -ForegroundColor Cyan
Write-Host " VALIDATION SUMMARY: $PassedCount Passed, $FailedCount Failed" -ForegroundColor $SummaryColor
Write-Host "=====================================================================" -ForegroundColor Cyan

if ($FailedCount -gt 0) {
    exit 1
}
exit 0
