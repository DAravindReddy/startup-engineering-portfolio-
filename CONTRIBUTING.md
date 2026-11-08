# Contributing to Startup Engineering Portfolio

Welcome to the **Startup Engineering Portfolio** repository maintained by **Aravind Reddy** ([@DAravindReddy](https://github.com/DAravindReddy)). 

This repository serves as the central engineering repository and institutional technical foundation for our technology startup. We follow rigorous engineering standards across Embedded Systems, Edge AI, Linux Systems Programming, and DevOps.

---

## 1. Code of Conduct

All contributors and team members are expected to adhere to our [Code of Conduct](CODE_OF_CONDUCT.md). We maintain a collaborative, respectful, and high-performance engineering culture.

---

## 2. Branching Strategy

We enforce a strict Git flow branching model:

```text
main (production-ready releases and validated master docs)
  └── develop (integration branch for staged features)
        ├── feature/<project-name>-<feature-description>
        ├── bugfix/<project-name>-<issue-fix>
        └── documentation/<project-name>-<doc-update>
```

- Direct pushes to `main` are restricted.
- All code enters via Pull Requests into `develop` or `main` after automated CI verification and code review.

---

## 3. Commit Message Standards

We adhere to the [Conventional Commits](https://www.conventionalcommits.org/) specification:

| Type | Intent | Example |
|---|---|---|
| `feat` | New feature or module | `feat(embedded): add I2C retry mechanism to DS1307 driver` |
| `fix` | Bug fix or hardware timing patch | `fix(linux): handle EINTR gracefully in n_pipe execution` |
| `docs` | Documentation and architecture updates | `docs(android): add telemetry pipeline sequence diagram` |
| `refactor` | Code restructuring without behavior change | `refactor(shared): extract ring buffer into shared embedded library` |
| `test` | Unit, integration, or hardware-in-the-loop tests | `test(ai): add unit tests for Isolation Forest risk classifier` |
| `ci` | CI/CD pipeline and automation changes | `ci(github): add matrix compilation workflow for GCC 16 and Clang` |
| `build` | Build system, CMake, or Makefile updates | `build(cmake): add CTest target for data structures test suite` |

---

## 4. Coding & Architecture Standards

Every project in this repository follows the strict documentation and source code layout outlined in `00_STARTUP_DOCUMENTATION/engineering_standards.md`:

1. **C / C++ Standards:**
   - C11 / C++17 or newer standards.
   - Zero compiler warnings with `-Wall -Wextra -Werror -pedantic`.
   - Strict memory management: no memory leaks, no dangling pointers, clean RAII patterns.
   - Clean header (`include/`) and source (`src/`) separation.

2. **Python & AI/ML Standards:**
   - Python 3.10+ compliant code conforming to PEP 8.
   - Type hints throughout public functions and classes.
   - Isolated `requirements.txt` or `pyproject.toml` per project.
   - Separation of data acquisition, feature extraction, model training, and inference.

3. **Embedded & Arduino Standards:**
   - Non-blocking state machines over arbitrary `delay()` loops.
   - Hardware abstraction layers isolating register-level logic from application state.
   - Documented pin configurations, timing diagrams, and sensor tolerances.

4. **Documentation Standards:**
   - Every major project must have an architectural blueprint, data flow diagram (Mermaid), technical specification, installation guide, and formal whitepaper.

---

## 5. Submitting a Pull Request

1. Fork or create a dedicated feature branch from `develop`.
2. Ensure all unit tests pass locally.
3. Validate compilation (`gcc`, `make`, `cmake`, or `python -m pytest`).
4. Ensure no binary artifacts or credentials are staged.
5. Submit a comprehensive Pull Request linking any relevant issues.
