/**
 * @file sensor_monitor.h
 * @brief High-reliability multi-sensor telemetry aggregator and statistical engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#ifndef SENSOR_MONITOR_H
#define SENSOR_MONITOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_BUFFER_CAPACITY 32

typedef struct {
    uint64_t timestamp_ms;
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
    float light_lux;
    float air_quality_ppm;
} sensor_frame_t;

typedef struct {
    float min_val;
    float max_val;
    float mean_val;
    float std_dev;
    uint32_t sample_count;
} channel_stats_t;

typedef struct {
    channel_stats_t temp_stats;
    channel_stats_t humidity_stats;
    channel_stats_t pressure_hpa_stats;
    channel_stats_t light_stats;
    channel_stats_t air_quality_stats;
} telemetry_summary_t;

typedef struct {
    float temp_high_warning;
    float temp_high_critical;
    float air_quality_high_warning;
    float air_quality_high_critical;
    float pressure_low_warning;
} sensor_thresholds_t;

typedef enum {
    ALERT_NONE            = 0x00,
    ALERT_TEMP_WARNING    = 0x01,
    ALERT_TEMP_CRITICAL   = 0x02,
    ALERT_AIR_WARNING     = 0x04,
    ALERT_AIR_CRITICAL    = 0x08,
    ALERT_PRESSURE_DROP   = 0x10
} alert_flags_t;

typedef struct {
    sensor_frame_t buffer[RING_BUFFER_CAPACITY];
    size_t head;
    size_t count;
    sensor_thresholds_t thresholds;
    uint32_t active_alerts;
    uint32_t total_frames_processed;
} sensor_monitor_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize sensor monitoring state structure.
 * @param monitor Pointer to monitor structure.
 */
void sensor_monitor_init(sensor_monitor_t *monitor);

/**
 * @brief Ingest a new sensor telemetry frame into the rolling window.
 * @param monitor Pointer to monitor structure.
 * @param frame Telemetry frame.
 */
void sensor_monitor_push_frame(sensor_monitor_t *monitor, const sensor_frame_t *frame);

/**
 * @brief Compute real-time statistical aggregations across the active window.
 * @param monitor Pointer to monitor structure.
 * @param summary Pointer to summary struct to populate.
 */
void sensor_monitor_compute_stats(const sensor_monitor_t *monitor, telemetry_summary_t *summary);

/**
 * @brief Evaluate active alerts based on the latest frame.
 * @param monitor Pointer to monitor structure.
 * @return Active alert flags bitmask.
 */
uint32_t sensor_monitor_check_alerts(sensor_monitor_t *monitor);

/**
 * @brief Format latest telemetry and statistics into a JSON string.
 * @param monitor Pointer to monitor structure.
 * @param json_buffer Output buffer.
 * @param max_len Maximum length of buffer.
 * @return True on successful formatting, false on buffer overflow.
 */
bool sensor_monitor_format_json(const sensor_monitor_t *monitor, char *json_buffer, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MONITOR_H */
