# Embedded Linux & Yocto Project Engineering Suite

**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Organization:** Kernelwise Labs  
**Domain:** Embedded Linux, Board Support Packages (BSP), Yocto BitBake, Cross-Compilation  

---

## 1. Overview

This directory houses the complete Embedded Linux engineering framework developed by Kernelwise Labs. It integrates:
1. **Original Academic & Training Assets:** Preserved initial digital watch prototyping and Yocto presentation documentation (`original_watch_app/` and `yocto_docs/`).
2. **Production Digital Watch Application (`digital_watch_system/`):** High-precision C11 multi-threaded timekeeping, lap-recording stopwatch, countdown timer, and alarm subsystem with ANSI terminal visualization.
3. **Custom Yocto Meta Layer (`meta-watch/`):** Production BitBake recipes, systemd service units, and minimal image generation recipes (`embedded-watch-image.bb`).
4. **Toolchains & Automation (`cross_toolchain_and_bsp/`):** Automated toolchain setup, BitBake compilation orchestrator, and block-device SD card partitioning utilities.
5. **Architectural Guides (`documentation/`):** Detailed papers on the multi-stage embedded boot sequence, BitBake metadata hierarchy, and cross-compilation toolchains.

---

## 2. Directory Structure

```
05_LINUX_AND_SYSTEMS/embedded_linux/
│
├── README.md
├── LICENSE
│
├── original_watch_app/                 # Preserved original educational watch codebase
│   ├── Makefile
│   └── src/watch.c
│
├── yocto_docs/                         # Preserved original presentation material
│   └── Yocto_ppt.odt
│
├── digital_watch_system/               # Production C11 digital watch engine
│   ├── Makefile
│   ├── include/
│   │   └── digital_watch.h
│   ├── src/
│   │   ├── watch_core.c
│   │   ├── display.c
│   │   └── main.c
│   └── tests/
│       └── test_watch_core.c
│
├── meta-watch/                         # BitBake custom layer for Yocto
│   ├── conf/
│   │   └── layer.conf
│   ├── recipes-apps/
│   │   └── digital-watch/
│   │       ├── digital-watch_1.0.bb
│   │       └── files/digital-watch.service
│   └── recipes-core/
│       └── images/
│           └── embedded-watch-image.bb
│
├── cross_toolchain_and_bsp/            # Automation scripts
│   └── scripts/
│       ├── setup_cross_toolchain.sh
│       ├── build_yocto_image.sh
│       └── flash_sd_card.sh
│
└── documentation/                      # Technical references & architecture
    ├── embedded_linux_architecture.md
    ├── yocto_build_system_guide.md
    └── cross_compilation_guide.md
```

---

## 3. Quickstart & Verification

### Build & Run Unit Tests (Native C11)
```bash
cd digital_watch_system
make test
```
*Expected Output:*
```
[PASS] test_initialization
[PASS] test_stopwatch_laps
[PASS] test_countdown_timer
[PASS] test_alarm_subsystem
ALL UNIT TESTS PASSED SUCCESSFULLY (100%)
```

### Run Demonstration
```bash
cd digital_watch_system
make
./digital_watch --demo
```

### Cross-Compilation for ARM
```bash
source cross_toolchain_and_bsp/scripts/setup_cross_toolchain.sh armhf
cd digital_watch_system
make CROSS_COMPILE=arm-linux-gnueabihf-
```
