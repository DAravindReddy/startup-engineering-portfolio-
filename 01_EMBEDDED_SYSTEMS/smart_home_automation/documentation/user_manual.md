# User Operating Manual: Bluetooth Terminal & Control

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Bluetooth Pairing Instructions

1. Power the Arduino UNO and 12V supply. The red LED on the HC-05 will flash rapidly (~2 Hz), indicating pairing mode.
2. Enable Bluetooth on your smartphone or PC.
3. Scan for nearby devices and select **`HC-05`**.
4. Enter pairing PIN: **`1234`** (or `0000`).
5. Once paired, launch any standard Bluetooth Serial Terminal app (e.g., *Serial Bluetooth Terminal* by Kai Morich on Android, or PuTTY on Windows).
6. Connect to `HC-05`. The onboard LED on the HC-05 will transition to a slow double-blink (~0.5 Hz).

---

## 2. Command Reference Table

All commands must be terminated by a newline (`\n` or `\r\n`).

| Command String | Parameters | Subsystem | Action & Response |
|---|:---:|---|---|
| `L1_ON` | None | Appliance 1 | Energizes Room Light indicator LED. Response: `[ACK] LED1 ON` |
| `L1_OFF` | None | Appliance 1 | De-energizes Room Light LED. Response: `[ACK] LED1 OFF` |
| `L2_ON` | None | Appliance 2 | Energizes Yard Light indicator LED. Response: `[ACK] LED2 ON` |
| `L2_OFF` | None | Appliance 2 | De-energizes Yard Light LED. Response: `[ACK] LED2 OFF` |
| `M_ON` | None | Ventilation | Activates DC motor at default speed 200 PWM. Response: `[ACK] MOTOR ON` |
| `M_OFF` | None | Ventilation | Stops DC motor. Response: `[ACK] MOTOR OFF` |
| `M_SPEED:<VAL>` | `0 - 255` | Ventilation | Sets motor PWM speed. Example: `M_SPEED:180`. Response: `[ACK] MOTOR SPEED: 180` |
| `SERVO:<ANGLE>` | `0 - 180` | Window Vent | Positions servo damper. Example: `SERVO:90`. Response: `[ACK] SERVO ANGLE: 90` |
| `PUMP_ON` | None | Irrigation | Manually forces water pump ON. Response: `[ACK] MANUAL PUMP ON` |
| `PUMP_OFF` | None | Irrigation | Manually stops water pump. Response: `[ACK] MANUAL PUMP OFF` |
| `MODE_AUTO` | None | Irrigation | Enables closed-loop autonomous irrigation. Response: `[ACK] IRRIGATION MODE: AUTO` |
| `MODE_MANUAL` | None | Irrigation | Disables auto soil triggering. Response: `[ACK] IRRIGATION MODE: MANUAL` |
| `SET_TH:<PCT>` | `5 - 95` | Irrigation | Configures dry trigger threshold %. Example: `SET_TH:40`. Response: `[ACK] SOIL DRY THRESHOLD SET TO: 40` |
| `STATUS?` | None | System | Emits immediate formatted JSON telemetry string. |

---

## 3. Interpreting Telemetry JSON Output

The system broadcasts a JSON telemetry payload every 3 seconds:

```json
{
  "temp": 26.5,
  "hum": 54.0,
  "soil_pct": 48,
  "pump": 0,
  "mode": "AUTO",
  "led1": 1,
  "led2": 0,
  "motor_spd": 180,
  "servo": 90
}
```
