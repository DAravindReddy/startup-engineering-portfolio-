# Multi-Sensor Monitoring Node - Technical Documentation

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  

---

## 1. Functional Specifications

1. **Environmental Multi-Modal Ingestion:** Simultaneously captures 5 physical environmental dimensions: temperature, humidity, atmospheric pressure, illuminance, and air quality index.
2. **Deterministic Ring Buffering:** Employs a non-allocating, zero-fragmentation static ring buffer in SRAM storing $N=32$ historical frames.
3. **Real-Time Statistical Computing:** Computes running minima, maxima, sample means, and standard deviations on-chip without floating-point division errors.
4. **Hierarchical Alert Thresholding:** Evaluates dual-tier alert conditions (warning vs critical) for rapid mitigation.
5. **Standardized MQTT Uplink:** Formats JSON telemetry compatible with Home Assistant, ThingsBoard, and AWS IoT Core.

---

## 2. API Reference

- `void sensor_monitor_init(sensor_monitor_t *monitor)`: Zeroes state, configures alert thresholds.
- `void sensor_monitor_push_frame(sensor_monitor_t *monitor, const sensor_frame_t *frame)`: Inserts new frame into circular buffer.
- `void sensor_monitor_compute_stats(const sensor_monitor_t *monitor, telemetry_summary_t *summary)`: Computes statistical metrics across buffered frames.
- `uint32_t sensor_monitor_check_alerts(sensor_monitor_t *monitor)`: Flags active warning and critical conditions.
- `bool sensor_monitor_format_json(const sensor_monitor_t *monitor, char *json_buffer, size_t max_len)`: Emits compact JSON string.

---

## 3. Troubleshooting Matrix

| Issue | Root Cause | Solution |
| :--- | :--- | :--- |
| BMP280 not detected on I2C | Incorrect I2C address (`0x76` vs `0x77`) | Connect SDO pin to GND for `0x76` or VDD for `0x77` |
| MQ-135 values fluctuate wildly | Insufficient preheat burn-in | Allow internal heater coil to burn in for 24–48 hours |
| JSON output truncated | Output buffer length `< 256` bytes | Allocate $\ge 512$ bytes for `json_buffer` |
