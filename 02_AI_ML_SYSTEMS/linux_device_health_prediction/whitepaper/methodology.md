# Methodology: Isolation Forest Kernel Telemetry Formulation

## 1. Feature Representation
Telemetry is structured into an 18-dimensional feature vector $\mathbf{x} \in \mathbb{R}^{18}$:
- CPU & Scheduler: User %, System %, IO-Wait %, 1m Load, Context Switches/sec.
- Memory: RAM used %, Swap used %.
- Thermal: Peak SoC Temperature, Throttling State.
- Kernel Statistics: Interrupts/sec, Running Procs, Blocked Procs (D-state), Zombie Count.
- Storage & Network: Disk Write kB/sec, Network RX Drops/sec, UART Errors.
- Dynamic Rates: $\Delta T_{\text{SoC}} / \Delta t$, $\Delta \text{IRQ} / \Delta t$.

## 2. Model Training Procedure
1. **Nominal Sampling:** Generate/acquire 1,500 baseline operational cycles under normal load variations.
2. **Isolation Forest Hyperparameters:**
   - Ensemble Size: $T = 100$ isolation trees.
   - Subsampling Size: $\psi = 256$ samples per tree.
   - Contamination Factor: $c = 0.03$ (assuming 3% nominal noise margin).
   - Execution Mode: Single-threaded sequential training (`n_jobs=1`) ensuring deterministic cross-platform execution.
3. **Calibrated Continuous Risk Mapping:**
   $$\text{RiskScore}(x) = \frac{100.0}{1 + \exp\left(18.0 \cdot D(x)\right)}$$
