# Advanced C Programming & Low-Level Systems Engineering (C11)

[![Standard: C11](https://img.shields.io/badge/Standard-C11-blue.svg)](README.md)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Compiler: GCC / Clang](https://img.shields.io/badge/Compiler-GCC%20%2F%20Clang-green.svg)](Makefile)

A comprehensive repository of systems-level C11 implementations covering bitwise register manipulation, pointer arithmetic, memory layout, custom file stream handlers, and user-defined types (UDT / structs / unions).

---

## Technical Domains & Architectural Coverage

### 1. Bitwise Operations & Hardware Register Manipulation (`bitwise/`)
Low-level bit manipulation is critical for microcontrollers and device drivers where hardware control registers map directly to bitfields:
- **Bitwise Set:** `REG |= (1U << n)`
- **Bitwise Clear:** `REG &= ~(1U << n)`
- **Bitwise Toggle:** `REG ^= (1U << n)`
- **Bitfield Extraction & Masking:** `(REG >> n) & 0x01`
- **Circular Bit Rotation & Parity Computation**

### 2. Pointers & Dynamic Memory Architecture (`pointers/`)
- Double pointers (`**ptr`) for dynamic matrix allocation and linked list head indirection.
- Function pointers for callback registration and virtual method tables (vtable dispatch).
- Pointer arithmetic rules and alignment boundaries in virtual memory.

### 3. File Streams & Binary I/O (`Files/`)
- Block-buffered binary serialization (`fread`, `fwrite`).
- Stream positioning (`fseek`, `ftell`, `rewind`) and file truncation.
- Robust error handling with `ferror` and `feof`.

### 4. Strings & Memory Buffers (`strings/`)
- Zero-copy string traversal.
- Safe buffer manipulation (`snprintf`, `strncmp`, `strtok_r`).
- Re-entrant parsing algorithms.

### 5. User-Defined Types (`UDT/`)
- `struct` memory alignment, padding bytes, and packing attributes (`__attribute__((packed))`).
- `union` type punning and shared memory registers.
- `typedef enum` state representation.

---

## Directory Structure

```text
C_programming/
├── README.md                          # Master technical overview
├── LICENSE                            # MIT License
├── storage-class.txt                  # auto, register, static, extern memory lifetimes
│
├── bitwise/                           # Bitwise algorithms & register masks
├── pointers/                          # Pointer arithmetic & function callbacks
├── Files/                             # File stream I/O and serialization
├── strings/                           # String algorithms and buffer safety
└── UDT/                               # Structs, Unions, and Bitfield packing
```

---

## Author & Attribution
Authored by **Aravind Reddy** ([@DAravindReddy](https://github.com/DAravindReddy)).  
Standardized and curated for Kernelwise Labs portfolio monorepo.
