# Modern C++ Systems Programming (C++17)

[![Standard: C++17](https://img.shields.io/badge/Standard-C%2B%2B17-blue.svg)](README.md)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Compiler: GCC / Clang](https://img.shields.io/badge/Compiler-GCC%20%2F%20Clang-green.svg)](Makefile)

A curated repository of modern C++17 design patterns, concurrency idioms, RAII abstractions, and high-performance algorithms for systems programming.

---

## Core Paradigms Implemented

### 1. RAII & Resource Ownership (Rule of Five)
- Explicit resource acquisition in constructors, deterministic reclamation in destructors.
- Move constructors and move assignment operators eliminating deep copies.

### 2. Lock-Based Multi-Threaded Task Pool (`include/thread_pool.hpp`)
- Fixed-size worker pool executing tasks using `std::future`, `std::packaged_task`, and `std::condition_variable`.
- Perfectly-forwarded task arguments (`std::forward<Args>`).

### 3. Functional STL Algorithms & Lambdas
- High-efficiency functional reductions with `std::accumulate`, `std::iota`, and non-allocating lambdas.

---

## Directory Structure

```text
CPP_programming/
├── README.md                          # Master overview
├── LICENSE                            # MIT License
│
├── include/
│   └── thread_pool.hpp                # Thread-safe task pool implementation
└── src/
    └── main.cpp                       # Comprehensive demonstration suite
```

---

## Compilation

```bash
g++ -Wall -Wextra -std=c++17 src/main.cpp -o cpp_demo
./cpp_demo
```
