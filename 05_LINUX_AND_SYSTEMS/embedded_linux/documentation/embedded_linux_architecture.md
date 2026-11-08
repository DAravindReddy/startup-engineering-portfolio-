# Embedded Linux System Architecture & Boot Sequence

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  
**Standard:** IEEE 1471 / ISO/IEC 42010 Architecture Documentation  

---

## 1. Executive Summary

This document delineates the four-stage boot pipeline, memory address mapping, and user-space initialization topology implemented in the Kernelwise Labs Embedded Linux Platform. 

```
+-----------------------------------------------------------------------------------+
|                            EMBEDDED LINUX BOOT LIFECYCLE                          |
+-------------------+     +-------------------+     +-------------------------------+
|  Phase 1: ROM     | --> |  Phase 2: SPL     | --> |  Phase 3: U-Boot Proper       |
|  Internal BootROM |     |  Secondary Loader |     |  DRAM init, DTB loading, ENV  |
+-------------------+     +-------------------+     +-------------------------------+
                                                                    |
                                                                    v
+-------------------+     +-------------------+     +-------------------------------+
|  Phase 6: Apps    | <-- |  Phase 5: Init    | <-- |  Phase 4: Linux Kernel        |
|  digital_watch    |     |  systemd PID 1    |     |  zImage/Image + DTB parsing   |
+-------------------+     +-------------------+     +-------------------------------+
```

---

## 2. The Four-Stage Boot Sequence

### Stage 1: Primary BootROM (Hardware Level)
1. Hardwired mask ROM executes immediately upon system reset.
2. Checks strapping pins (boot selection switches / resistors) to determine primary boot medium (eMMC, SD card, SPI Flash, UART).
3. Reads first sectors of boot device into Internal Static RAM (SRAM, typically 64KB - 128KB).
4. Transfers control to Secondary Program Loader (SPL / MLO).

### Stage 2: Secondary Program Loader (SPL / MLO)
1. Operates within the constrained physical SRAM.
2. Initializes the external DRAM controller (DDR3/DDR4 timing, calibration, impedance matching).
3. Configures basic Phase-Locked Loops (PLLs) to scale processor core clocks.
4. Reads U-Boot proper (`u-boot.img`) from flash into external DDR RAM and executes it.

### Stage 3: U-Boot Proper (Universal Bootloader)
1. Relocates itself to high DDR memory.
2. Initializes console serial bus (UART), network PHY, USB, and MMC interfaces.
3. Evaluates stored environment variables (`uEnv.txt` or SPI flash sector).
4. Loads kernel binary (`zImage` / `Image`) and flattened Device Tree Blob (`.dtb`) into memory:
   ```bash
   fatload mmc 0:1 0x82000000 zImage
   fatload mmc 0:1 0x88000000 am335x-boneblack.dtb
   bootz 0x82000000 - 0x88000000
   ```

### Stage 4: Linux Kernel Initialization & Virtual Memory
1. Decompresses kernel executable (if `zImage`).
2. Architecture-dependent early setup (`head.S`): Enables MMU and data caches, establishes virtual-to-physical memory page translation.
3. Parses flattened device tree blob to enumerate hardware buses, interrupts, GPIOs, and peripheral base addresses.
4. Initializes kernel subsystems: SLUB memory allocator, VFS, workqueues, scheduler (CFS/EEVDF).
5. Mounts root filesystem specified by command line parameters (`root=/dev/mmcblk0p2 rw rootwait`).
6. Spawns PID 1 (`/sbin/init` or `/lib/systemd/systemd`).

---

## 3. Kernelwise User-Space Execution Model

In our target architecture, `systemd` acts as PID 1, managing daemons via cgroups:
- `digital-watch.service`: Multi-threaded embedded timekeeper and timer engine.
- `dropbear`: Lightweight secure remote shell daemon.
- `dbus-daemon`: Inter-process messaging bus for system state updates.
