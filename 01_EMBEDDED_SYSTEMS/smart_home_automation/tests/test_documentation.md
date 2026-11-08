# Testing Methodology & Test Documentation

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Test Levels & Strategy

Testing is conducted across two complementary tiers:
1. **Tier 1: Native C++ Unit Testing (Development Host):**
   - Compiles pure C/C++ logic modules (`bluetooth_handler.cpp`, `sensor_manager.cpp`, `irrigation_system.cpp`, `actuator_controller.cpp`) using GCC / Clang on PC.
   - Executes exhaustive parameter boundary checks, string tokenization fuzzing, and state machine transition tests.
2. **Tier 2: Hardware-in-the-Loop (HIL) Integration Testing:**
   - Validates live UART communication via Python serial client (`scripts/mock_bluetooth_client.py`).
   - Verifies physical pin voltages, PWM frequencies, and relay contact states.

---

## 2. Unit Test Suite Matrix (`test_smart_home.cpp`)

| Test Case ID | Target Module | Test Description | Input / Condition | Expected Outcome | Result |
|:---:|---|---|---|---|:---:|
| **TC-BT-01** | `bluetooth_handler` | Command parsing for appliance LEDs | `"L1_ON\r\n"`, `"L2_OFF\n"` | `CMD_LED1_ON`, `CMD_LED2_OFF` | **PASS** |
| **TC-BT-02** | `bluetooth_handler` | Parameterized motor speed | `"M_SPEED:185\n"` | `CMD_MOTOR_SPEED`, `param_val=185` | **PASS** |
| **TC-BT-03** | `bluetooth_handler` | Parameterized servo angle | `"SERVO:120\n"` | `CMD_SERVO_ANGLE`, `param_val=120` | **PASS** |
| **TC-BT-04** | `bluetooth_handler` | Parameterized soil threshold | `"SET_TH:42\n"` | `CMD_SET_THRESHOLD`, `param_val=42` | **PASS** |
| **TC-BT-05** | `bluetooth_handler` | Telemetry request query | `"STATUS?\n"` | `CMD_GET_STATUS` | **PASS** |
| **TC-BT-06** | `bluetooth_handler` | Garbage / invalid command rejection | `"UNKNOWN_XYZ\n"` | `CMD_UNKNOWN` | **PASS** |
| **TC-SOIL-01**| `sensor_manager` | Soil dry saturation boundary | Raw ADC $\ge 800$ | Returns exactly `0%` | **PASS** |
| **TC-SOIL-02**| `sensor_manager` | Soil water immersion boundary | Raw ADC $\le 300$ | Returns exactly `100%` | **PASS** |
| **TC-SOIL-03**| `sensor_manager` | Soil linear midpoint mapping | Raw ADC = 550 | Returns exactly `50%` | **PASS** |
| **TC-IRR-01** | `irrigation_system`| Auto mode dry threshold trigger | Moisture drops to 30% (< 35%) | Pump turns ON (`PUMP_STATE_WATERING`) | **PASS** |
| **TC-IRR-02** | `irrigation_system`| Hysteresis band holding | Moisture rises to 45% (between 35-65%) | Pump remains ON (Does not chatter) | **PASS** |
| **TC-IRR-03** | `irrigation_system`| Wet threshold cutoff | Moisture reaches 68% (>= 65%) | Pump turns OFF (`PUMP_STATE_COOLDOWN`)| **PASS** |
| **TC-IRR-04** | `irrigation_system`| Safety Watchdog Timeout Cutoff | Pump runs for $> 15,000$ ms | Emergency trip (`PUMP_STATE_SAFETY_TRIP`)| **PASS** |
