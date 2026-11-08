# Software Design Specification: Car Black Box

**Project:** Automotive Event Data Recorder  

---

## 1. Memory Organization & EEPROM Map

The external 24C02 EEPROM is partitioned as follows:

```text
EEPROM Memory Map (24C02 - 256 Bytes Total):
-------------------------------------------------------------------------------------
Address Range   | Length   | Field Description
-------------------------------------------------------------------------------------
0x00 - 0x03     | 4 Bytes  | Master Security PIN (Default: '1', '0', '1', '0')
0x04            | 1 Byte   | Current Event Log Count (0 to 10)
0x05 - 0x09     | 5 Bytes  | Reserved System Configuration Flags
-------------------------------------------------------------------------------------
0x0A - 0x13     | 10 Bytes | Log Entry #0: [HHMMSS][EV][SP]
0x14 - 0x1D     | 10 Bytes | Log Entry #1: [HHMMSS][EV][SP]
0x1E - 0x27     | 10 Bytes | Log Entry #2: [HHMMSS][EV][SP]
...             | ...      | ...
0x64 - 0x6D     | 10 Bytes | Log Entry #9: [HHMMSS][EV][SP]
-------------------------------------------------------------------------------------
0x6E - 0xFF     | 146 Bytes| Free / Unallocated Expansion Space
```

---

## 2. Event Code Dictionary

| Event String | Operational Scenario | Trigger Mechanism |
|:---:|---|---|
| `"ON"` | System Ignition ON / Microcontroller Boot | Reset vector initialization |
| `"GN"` | Transmission shifted into Neutral | Keypad SW2 / SW3 shift sequence |
| `"GR"` | Transmission shifted into Reverse | Keypad shift down from Neutral |
| `"G1"` | 1st Gear Engaged | Keypad shift up from Neutral |
| `"G2"` | 2nd Gear Engaged | Keypad shift up from 1st |
| `"G3"` | 3rd Gear Engaged | Keypad shift up from 2nd |
| `"G4"` | 4th Gear Engaged | Keypad shift up from 3rd |
| `"BR"` | Sudden Braking Applied | Keypad SW4 press |
| `"C_"` | Collision / Severe Deceleration Impact | Keypad SW1 emergency trigger |
