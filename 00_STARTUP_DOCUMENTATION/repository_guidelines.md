# Startup Engineering Repository Guidelines

**Organization:** Kernelwise Labs / Startup Engineering Monorepo  
**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Audience:** Startup Engineers, Maintainers, and External Contributors  

---

## 1. Purpose of the Central Monorepo

The `startup-engineering-portfolio` repository serves as the single source of truth for all software, firmware, AI models, hardware schematics, and technical research developed within our startup ecosystem. 

By centralizing diverse projects—spanning 8-bit microcontrollers, Linux kernel modules, and edge neural networks—under a unified structure, we maximize code reuse, maintain strict architectural consistency, and provide institutional investors and clients with clear evidence of technical maturity.

---

## 2. Project Lifecycle & Status Classifications

Every project folder MUST maintain a `PROJECT_STATUS.md` file in its root. No project may be claimed as "Production Ready" without passing the empirical criteria defined below:

| Status Stage | Definition & Criteria | Permissible Claims |
|---|---|---|
| **1. Idea** | Conceptual architecture, initial market requirement, high-level feasibility. | Conceptual whitepaper drafts only. |
| **2. Research** | Literature review, sensor bench evaluation, algorithmic simulation. | Math formulations, synthetic data tests. |
| **3. In Development** | Active code generation, driver implementation, API scaffolding. | Unit test execution, WIP status. |
| **4. Prototype** | Working proof-of-concept on breadboard or development board. | Demonstrated functional loop, preliminary timings. |
| **5. Testing** | Formal verification, stress testing, thermal chamber/HIL tests. | Empirical metrics, failure-mode analysis. |
| **6. Completed** | Fully validated against functional requirements with complete docs. | Validated project baseline with verified benchmarks. |
| **7. Production Ready** | Hardened, tested across operating extremes, commercial BOM, zero known security flaws. | Enterprise / commercial client deployment. |

> [!CAUTION]
> **Strict Anti-Fabrication Rule:** Under no circumstances should test results, training accuracy, dataset sizes, or failure predictions be fabricated. If a metric has not been empirically recorded on physical hardware or real telemetry traces, it must be marked as `Theoretical / Pending Verification`.

---

## 3. Organizational Directory Taxonomy

To prevent repository bloat and maintain navigational clarity:

1. `00_STARTUP_DOCUMENTATION/`: High-level corporate strategy, architectural standards, roadmaps, and tech stacks.
2. `01_EMBEDDED_SYSTEMS/`: Production and prototype micro-controller systems (bare-metal, RTOS).
3. `02_AI_ML_SYSTEMS/`: Predictive health models, anomaly isolation engines, and time-series pipelines.
4. `03_IOT_AND_AUTOMATION/`: Sensor acquisition nodes, telemetry gateways, closed-loop environmental controls.
5. `04_SOFTWARE_ENGINEERING/`: Fundamental computer science, data structures, algorithms, and modular C/C++ libraries.
6. `05_LINUX_AND_SYSTEMS/`: POSIX system programming, kernel drivers, sysfs telemetry, and OS bring-up.
7. `06_EXISTING_GITHUB_PROJECTS/`: Imported and preserved historical repositories with full attribution and enhanced documentation.
8. `07_RESEARCH_AND_WHITEPAPERS/`: Publication-ready technical whitepapers consolidating multidisciplinary research.
9. `08_SHARED_LIBRARIES/`: Decoupled, reusable drivers (I2C, UART, SPI), ring buffers, protocol decoders, and ML utility libraries.
10. `09_TOOLS_AND_AUTOMATION/`: Tooling, build scripts, mock telemetry simulators, and GitHub Actions CI pipelines.
11. `10_TEMPLATES/`: Canonical boilerplate directories for launching new embedded, AI/ML, Linux, and robotics modules.

---

## 4. Code Review & Quality Gates

Before any pull request is merged into `develop` or `main`:

1. **Compilation Check:** C/C++ code must compile warning-free under `-Wall -Wextra -Werror -pedantic`.
2. **Automated Testing:** Python suites must pass `pytest` with >80% coverage; C suites must pass CTest / custom test runners.
3. **Documentation Completeness:** PRs modifying features must update the respective `README.md`, `system_architecture.md`, and `technical_documentation.md`.
4. **Secret Scanning:** No personal access tokens, API keys, passwords, or company-confidential IP may be committed.
5. **Diagram Validation:** All Mermaid diagrams must render without syntax errors in standard GitHub markdown viewers.
