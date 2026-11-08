# Tools, Build & Testing Automation Domain

**Maintainer:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Organization:** Kernelwise Labs  
**Standard:** Continuous Integration, Automated Unit Testing, Matrix Builds  

---

## 1. Domain Overview

The `09_TOOLS_AND_AUTOMATION` domain provides the centralized developer tooling, CI/CD pipelines, compilation orchestrators, and automated test runners that enforce quality gates across the entire monorepo.

```
09_TOOLS_AND_AUTOMATION/
│
├── README.md
│
├── github_workflows/
│   └── ci.yml                      # Production GitHub Actions matrix CI/CD workflow
│
├── scripts/
│   ├── validate_repository.ps1     # Native Windows PowerShell validation script
│   └── validate_repository.sh      # Native POSIX / Linux / macOS validation script
│
└── testing_automation/
    └── run_all_tests.py            # Universal cross-platform multi-language test orchestrator
```

---

## 2. CI/CD Pipeline Architecture (`.github/workflows/ci.yml`)

The GitHub Actions workflow executes on every commit and pull request targeting `main`, `master`, and `develop`:
1. **C/C++ Matrix Build:**
   - Compilers: `gcc`, `clang`.
   - Compiles all shared C libraries (`i2c_hal`, `uart_hal`, `dht11_driver`, `packet_protocol`) under `-Wall -Wextra -Werror -pedantic -std=c11`.
   - Builds and runs all unit test suites for Embedded Systems, Robotics, IoT nodes, and Linux Digital Watch under `-std=c++14` and `-std=c++17`.
2. **Python Telemetry & Machine Learning Matrix:**
   - Interpreters: Python 3.10, 3.11, 3.12.
   - Runs `unittest discover` across Android and Linux Isolation Forest AI modules.

---

## 3. How to Run Validation Locally

### On Windows (PowerShell):
```powershell
powershell -ExecutionPolicy Bypass -File 09_TOOLS_AND_AUTOMATION\scripts\validate_repository.ps1
```

### On Linux / macOS (Bash):
```bash
bash 09_TOOLS_AND_AUTOMATION/scripts/validate_repository.sh
```

### Cross-Platform via Python:
```bash
python 09_TOOLS_AND_AUTOMATION/testing_automation/run_all_tests.py
```
*Expected Result:*
```
SUMMARY: 13 Passed, 0 Failed
```
