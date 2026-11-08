# User Operating Manual: Car Black Box

**Project:** Automotive Event Data Recorder  

---

## 1. Dashboard View (Normal Driving Mode)

Upon vehicle ignition, the 16x2 CLCD initializes into the real-time driving dashboard:

```text
+----------------+
|  TIME    EV  SP|  <- Line 1: Real-time Clock, Event Code, Speed (km/h)
|14:32:05  G3  65|  <- Line 2: 14:32:05, Gear 3, 65 km/h
+----------------+
```

### Controls in Dashboard Mode:
- **SW1 (RB0):** Trigger Collision Event (`C_`). Logs event with current speed and timestamp.
- **SW2 (RB1):** Increment Gear (`G1` -> `G2` -> `G3` -> `G4`).
- **SW3 (RB2):** Decrement Gear (`G4` -> `G3` -> `G2` -> `G1` -> `GN` -> `GR`).
- **SW4 (RB3):** Apply Brakes (`BR`). Logs braking event.
- **SW5 (RB4):** Enter Menu / Password Login.

---

## 2. Password Authentication & Menu Navigation

Pressing **SW5** prompts for the 4-digit security PIN:

```text
+----------------+
| ENTER PASSWORD |
|      _ _ _ _   |
+----------------+
```
- **Default Master PIN:** `1010`
- **SW1:** Enter `'1'`
- **SW2:** Enter `'0'`
- **Attempts Allowed:** 3. Upon 3 consecutive failures, the system locks out all menu access for 60 seconds with an audible countdown timer.

### Menu Options:
1. **View Log:** Scroll through all recorded driving events with index, time, event code, and speed.
2. **Clear Log:** Purges the circular EEPROM buffer and resets log count to zero.
3. **Download Log:** Transmits the complete event audit trail over the UART serial port at 9600 baud.
4. **Change Password:** Permits authenticated operator to re-program the 4-digit security PIN.
5. **Set Time:** Permits in-system synchronization of DS1307 hours and minutes registers.
