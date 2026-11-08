/**
 * @file sensor_monitor.c
 * @brief Implementation of multi-sensor telemetry aggregator and statistical engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "sensor_monitor.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

void sensor_monitor_init(sensor_monitor_t *monitor) {
    if (!monitor) return;
    memset(monitor, 0, sizeof(sensor_monitor_t));

    // Default safety thresholds
    monitor->thresholds.temp_high_warning = 38.0f;
    monitor->thresholds.temp_high_critical = 50.0f;
    monitor->thresholds.air_quality_high_warning = 200.0f;
    monitor->thresholds.air_quality_high_critical = 400.0f;
    monitor->thresholds.pressure_low_warning = 950.0f;
}

void sensor_monitor_push_frame(sensor_monitor_t *monitor, const sensor_frame_t *frame) {
    if (!monitor || !frame) return;

    monitor->buffer[monitor->head] = *frame;
    monitor->head = (monitor->head + 1) % RING_BUFFER_CAPACITY;
    if (monitor->count < RING_BUFFER_CAPACITY) {
        monitor->count++;
    }
    monitor->total_frames_processed++;
}

static void compute_single_channel_stats(const float *values, size_t n, channel_stats_t *out) {
    if (n == 0) {
        memset(out, 0, sizeof(channel_stats_t));
        return;
    }

    float min_v = values[0];
    float max_v = values[0];
    double sum = 0.0;

    for (size_t i = 0; i < n; ++i) {
        if (values[i] < min_v) min_v = values[i];
        if (values[i] > max_v) max_v = values[i];
        sum += values[i];
    }

    float mean = (float)(sum / (double)n);
    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double diff = (double)values[i] - (double)mean;
        sum_sq_diff += diff * diff;
    }

    out->min_val = min_v;
    out->max_val = max_v;
    out->mean_val = mean;
    out->std_dev = (float)sqrt(sum_sq_diff / (double)n);
    out->sample_count = (uint32_t)n;
}

void sensor_monitor_compute_stats(const sensor_monitor_t *monitor, telemetry_summary_t *summary) {
    if (!monitor || !summary || monitor->count == 0) return;

    float t_arr[RING_BUFFER_CAPACITY];
    float h_arr[RING_BUFFER_CAPACITY];
    float p_arr[RING_BUFFER_CAPACITY];
    float l_arr[RING_BUFFER_CAPACITY];
    float a_arr[RING_BUFFER_CAPACITY];

    for (size_t i = 0; i < monitor->count; ++i) {
        t_arr[i] = monitor->buffer[i].temperature_c;
        h_arr[i] = monitor->buffer[i].humidity_pct;
        p_arr[i] = monitor->buffer[i].pressure_hpa;
        l_arr[i] = monitor->buffer[i].light_lux;
        a_arr[i] = monitor->buffer[i].air_quality_ppm;
    }

    compute_single_channel_stats(t_arr, monitor->count, &summary->temp_stats);
    compute_single_channel_stats(h_arr, monitor->count, &summary->humidity_stats);
    compute_single_channel_stats(p_arr, monitor->count, &summary->pressure_hpa_stats);
    compute_single_channel_stats(l_arr, monitor->count, &summary->light_stats);
    compute_single_channel_stats(a_arr, monitor->count, &summary->air_quality_stats);
}

uint32_t sensor_monitor_check_alerts(sensor_monitor_t *monitor) {
    if (!monitor || monitor->count == 0) return ALERT_NONE;

    size_t latest_idx = (monitor->head == 0) ? (RING_BUFFER_CAPACITY - 1) : (monitor->head - 1);
    const sensor_frame_t *latest = &monitor->buffer[latest_idx];

    uint32_t alerts = ALERT_NONE;

    if (latest->temperature_c >= monitor->thresholds.temp_high_critical) {
        alerts |= ALERT_TEMP_CRITICAL;
    } else if (latest->temperature_c >= monitor->thresholds.temp_high_warning) {
        alerts |= ALERT_TEMP_WARNING;
    }

    if (latest->air_quality_ppm >= monitor->thresholds.air_quality_high_critical) {
        alerts |= ALERT_AIR_CRITICAL;
    } else if (latest->air_quality_ppm >= monitor->thresholds.air_quality_high_warning) {
        alerts |= ALERT_AIR_WARNING;
    }

    if (latest->pressure_hpa <= monitor->thresholds.pressure_low_warning && latest->pressure_hpa > 0.0f) {
        alerts |= ALERT_PRESSURE_DROP;
    }

    monitor->active_alerts = alerts;
    return alerts;
}

bool sensor_monitor_format_json(const sensor_monitor_t *monitor, char *json_buffer, size_t max_len) {
    if (!monitor || !json_buffer || max_len == 0 || monitor->count == 0) return false;

    size_t latest_idx = (monitor->head == 0) ? (RING_BUFFER_CAPACITY - 1) : (monitor->head - 1);
    const sensor_frame_t *lat = &monitor->buffer[latest_idx];

    int written = snprintf(json_buffer, max_len,
        "{\"timestamp_ms\":%llu,\"telemetry\":{\"temp_c\":%.2f,\"humidity_pct\":%.2f,\"pressure_hpa\":%.2f,\"light_lux\":%.1f,\"air_quality_ppm\":%.1f},\"alerts\":%u,\"total_frames\":%u}",
        (unsigned long long)lat->timestamp_ms,
        lat->temperature_c,
        lat->humidity_pct,
        lat->pressure_hpa,
        lat->light_lux,
        lat->air_quality_ppm,
        monitor->active_alerts,
        monitor->total_frames_processed
    );

    return (written > 0 && (size_t)written < max_len);
}
