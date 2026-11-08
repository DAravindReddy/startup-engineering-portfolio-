# Linux Character Device Driver Suite

**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Organization:** Kernelwise Labs  
**Target Kernel:** Linux 5.x / 6.x  
**License:** Dual GPL-2.0 / MIT  

---

## 1. Overview

This directory provides an enterprise-grade Linux Loadable Kernel Module (LKM) Character Device Driver (`kernelwise_char`) adhering to modern kernel development practices.

Key features:
- **Dynamic Device Numbering:** Uses `alloc_chrdev_region()` to avoid major number collisions.
- **Sysfs & Udev Integration:** Automatically generates `/dev/kernelwise_char` via `class_create()` and `device_create()`.
- **VFS File Operations:** Full implementation of `open`, `release`, `read`, `write`, and `unlocked_ioctl`.
- **Fault-Tolerant Memory Transfers:** Rigorous exception handling using `copy_to_user()` and `copy_from_user()`.
- **Thread Safety:** Mutex synchronization (`DEFINE_MUTEX`) preventing concurrent corruption.
- **IOCTL Interface:** Command macros for buffer flushing, telemetry stat retrieval, and parameter reconfiguration.

---

## 2. Directory Structure

```
05_LINUX_AND_SYSTEMS/device_drivers/
│
├── README.md
├── LICENSE
├── Makefile
│
├── include/
│   └── char_driver_ioctl.h     # Shared user-kernel IOCTL definitions
│
├── src/
│   └── kernelwise_char.c       # Loadable Kernel Module (LKM) C source
│
├── tests/
│   └── test_user_app.c         # User-space verification application
│
└── documentation/
    └── char_device_driver_guide.md
```

---

## 3. Build & Deployment Instructions

### On Linux Host with Kernel Headers:
```bash
# Build module and user app
make

# Insert kernel module
make load

# Verify dmesg logs
dmesg | tail -n 10

# Run user-space test harness
make test

# Remove module
make unload
```
