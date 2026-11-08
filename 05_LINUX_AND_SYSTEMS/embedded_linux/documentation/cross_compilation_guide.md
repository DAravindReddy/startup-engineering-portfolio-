# Embedded Linux Cross-Compilation & Toolchains Guide

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  

---

## 1. Fundamentals of Cross-Compilation

In embedded systems development, the developer workstation (the **Host**) builds binaries targeting an embedded processor architecture (the **Target**) with different instruction sets, endianness, and C standard library implementations.

| Dimension | Host Machine | Target Embedded Device |
| :--- | :--- | :--- |
| **Architecture** | x86_64 (AMD64) | ARMv7-A (32-bit Cortex-A8) or AArch64 (Cortex-A53) |
| **Endianness** | Little Endian | Little Endian |
| **C Library** | GNU C Library (`glibc` 2.38+) | `musl` or lightweight `glibc` |
| **Floating Point** | SSE/AVX hardware registers | VFPv4 / NEON (`-mfloat-abi=hard -mfpu=neon`) |

---

## 2. Toolchain Triplet Breakdown

A cross-compiler is designated by a GNU architecture triplet:
`[arch]-[vendor]-[os]-[abi]`

Example: `arm-linux-gnueabihf-gcc`
- **`arm`**: Target architecture (ARM 32-bit).
- **`linux`**: Target Operating System kernel.
- **`gnu`**: User-space environment / libc flavor.
- **`eabihf`**: Embedded ABI with Hard-Float hardware register passing.

Example: `aarch64-linux-gnu-gcc`
- Target 64-bit ARM architecture where hardware floating-point registers (`d0-d31`) are mandated by the ARMv8 specification.

---

## 3. Sysroot & Library Linkage

When linking against external libraries (e.g. `pthread`, `libi2c`, `sqlite3`), the cross-linker must never search Host `/usr/lib` or `/usr/include`, as this leads to architecture mismatch errors:
`Relocations in generic ELF (EM: 62) ... cannot link x86_64 with ARM`.

Use `--sysroot`:
```bash
arm-linux-gnueabihf-gcc \
    --sysroot=/opt/kernelwise-toolchain/sysroots/cortexa8hf-neon-poky-linux-gnueabi \
    -O2 -Wall -Wextra src/main.c -o app -lpthread
```
