# Methodology & Algorithmic Formulations

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Soil Moisture Transformation Pipeline

The analog soil moisture probe generates a raw voltage $V_{\text{out}}$ inversely proportional to soil water content due to electrical conductivity changes.

### A. Mathematical Derivation
Let $D_{\text{raw}} \in [0, 1023]$ denote the raw 10-bit value returned by `analogRead(A0)`.  
Given empirical calibration endpoints $D_{\text{air}}$ (dry air count) and $D_{\text{water}}$ (submerged water count):

$$\Delta D = D_{\text{air}} - D_{\text{water}}$$

The calibrated volumetric percentage $P_{\text{soil}}$ is:

$$P_{\text{soil}} = \begin{cases}
0, & \text{if } D_{\text{raw}} \ge D_{\text{air}} \\
100, & \text{if } D_{\text{raw}} \le D_{\text{water}} \\
\left\lfloor \frac{D_{\text{air}} - D_{\text{raw}}}{\Delta D} \times 100 \right\rfloor, & \text{otherwise}
\end{cases}$$

### B. Integer Arithmetic Implementation
To avoid floating-point math overhead on 8-bit AVR hardware:

```c
uint8_t calculate_soil_moisture_pct(uint16_t raw_adc, uint16_t air_dry_adc, uint16_t water_wet_adc) {
    if (air_dry_adc <= water_wet_adc) return 0;
    if (raw_adc >= air_dry_adc) return 0;
    if (raw_adc <= water_wet_adc) return 100;
    uint32_t span = air_dry_adc - water_wet_adc;
    uint32_t val = air_dry_adc - raw_adc;
    return (uint8_t)((val * 100UL) / span);
}
```

---

## 2. Tokenized Command Parsing Complexity

The command parsing engine in `bluetooth_handler.cpp` processes strings in bounded $O(N)$ time where $N \le 32$ bytes. Commands are verified using string prefix matching, ensuring zero heap fragmentation (`malloc` is strictly prohibited).
