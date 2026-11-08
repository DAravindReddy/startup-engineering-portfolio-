# Linux Kernel Architecture & Development Suite

**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Organization:** Kernelwise Labs  
**Domain:** Operating System Kernels, Scheduler Internals, SLUB Memory, Device Trees, Syscalls  

---

## 1. Overview

This module contains comprehensive engineering documentation, implementation blueprints, and validation harnesses for Linux kernel development:
- **Custom System Call Implementation:** Adding, routing, and testing custom syscall vectors in kernel space.
- **Kernel Memory & Caches:** Deep analysis of the buddy page allocator, slab/slub caches, `kmalloc`, and `vmalloc`.
- **Device Tree Bindings:** Real-world examples of `.dts` hardware node definitions for I2C and SPI buses.
- **Scheduler & IRQ Internals:** Architectural breakdown of CFS, EEVDF, hard IRQs, and bottom-half workqueues.

---

## 2. Directory Structure

```
05_LINUX_AND_SYSTEMS/kernel_development/
│
├── README.md
├── LICENSE
│
├── src/
│   └── test_custom_syscall.c      # User space test harness for custom syscall
│
└── documentation/
    ├── custom_syscall_guide.md
    ├── kernel_memory_and_slub.md
    ├── device_tree_bindings.md
    └── scheduler_and_irq_subsystems.md
```

---

## 3. Verification

To verify the custom syscall test harness syntax:
```bash
gcc -Wall -Wextra -std=c11 src/test_custom_syscall.c -o test_syscall
./test_syscall
```
