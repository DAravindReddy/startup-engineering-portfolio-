# Embedded Project Template

Standard production template for bare-metal microcontroller and RTOS projects.

## Project Structure
- `source_code/`: Driver implementations (`src/`) and interface headers (`include/`).
- `hardware/`: Bill of materials (`components_list.md`), pinouts (`pin_configuration.md`), and wiring schematics.
- `architecture/`: System block diagrams, state transition models, and data flow specifications.
- `documentation/`: Functional requirements, software design, and troubleshooting guides.
- `whitepaper/`: Research whitepaper following IEEE format.
- `tests/`: Unit test suites and hardware-in-the-loop (HIL) verification plans.
- `results/`: Scope captures, logic analyzer traces, and performance benchmarks.
- `scripts/`: Flashing, compile, and static analysis automation.

## Quickstart
1. Copy this folder into your target directory.
2. Update `PROJECT_STATUS.md` with appropriate initial status (`Idea` or `In Development`).
3. Define pin mapping in `hardware/pin_configuration.md`.
