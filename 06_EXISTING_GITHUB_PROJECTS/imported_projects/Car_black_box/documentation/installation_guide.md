# Installation and Flashing Guide: Car Black Box

**Project:** Automotive Event Data Recorder  
**Target:** Microchip PIC16F877A  

---

## 1. Toolchain Requirements
- **Integrated Development Environment:** Microchip MPLAB X IDE (v5.45 or newer / v6.x).
- **C Compiler:** Microchip MPLAB XC8 C Compiler (v2.30+).
- **Hardware Programmer / Debugger:** PICkit 3, PICkit 4, or MPLAB SNAP.
- **Simulator / Hardware:** PIC16F877A development board or Proteus VSM simulation platform.

---

## 2. Project Setup in MPLAB X IDE

1. Open MPLAB X IDE.
2. Select **File -> New Project -> Microchip Embedded -> Standalone Project**.
3. Select Device: `PIC16F877A`.
4. Select Tool: `PICkit 3` (or `Simulator`).
5. Select Compiler: `XC8`.
6. Set Project Name: `Car_black_box`.
7. Add Source Files:
   - In `Projects` view, right click `Source Files -> Add Existing Items...`.
   - Select all `.c` files in `source_code/src/`.
8. Add Header Files:
   - In `Projects` view, right click `Header Files -> Add Existing Items...`.
   - Select all `.h` files in `source_code/include/`.
9. Configure Include Directories:
   - `Project Properties -> XC8 Global Options -> XC8 Compiler -> Include directories`.
   - Add `./source_code/include`.

---

## 3. Building and Flashing

```bash
# Clean and Build
# Press 'Clean and Build Project' (Hammer & Brush icon in MPLAB X)
# Or compile via command-line:
xc8-cc -mcpu=16F877A -I./source_code/include ./source_code/src/*.c -o Car_black_box.hex
```

Connect the PICkit to the ICSP header:
- Pin 1 (Arrow): `MCLR/VPP`
- Pin 2: `VDD (+5V)`
- Pin 3: `VSS (GND)`
- Pin 4: `ICSPDAT (RB7)`
- Pin 5: `ICSPCLK (RB6)`
- Pin 6: `NC`

Click **Make and Program Device**.
