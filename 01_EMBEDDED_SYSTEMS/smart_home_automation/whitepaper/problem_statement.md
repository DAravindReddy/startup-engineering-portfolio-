# Research Problem Statement & Operational Gaps

**Project:** Smart Home Automation with Bluetooth & Closed-Loop Irrigation  

---

## 1. Context and Technical Gaps

Traditional domestic automation and garden irrigation systems suffer from severe architectural fragmentation:

1. **Procedural Execution Blocking:** 
   Conventional embedded hobbyist sketches utilize blocking delay functions (such as `delay(2000)` during DHT11 sensor acquisition). During this execution window, the microcontroller is completely unresponsive to external asynchronous events, including wireless Bluetooth commands, sensor interrupts, and emergency stop triggers.
2. **Hysteresis Absence and Actuator Oscillation:**
   Basic soil moisture controllers apply naive single-point thresholds (e.g., $M < 40\% \implies \text{PUMP\_ON}$, $M \ge 40\% \implies \text{PUMP\_OFF}$). In natural soil, minor analog sensor noise and transient water absorption cause high-frequency relay cycling around the boundary, damaging mechanical relay contacts and causing inductive transients that reset the microcontroller.
3. **Catastrophic Flooding Risks from Sensor Disconnection:**
   Resistive and capacitive soil sensors installed outdoors are vulnerable to mechanical displacement (animals, wind, root growth) and corrosion. When a sensor wire disconnects, the analog reading registers maximum resistance (indicating 0% moisture). An unmitigated closed-loop controller will run the pump indefinitely, emptying water reservoirs and destroying household or agricultural assets.
