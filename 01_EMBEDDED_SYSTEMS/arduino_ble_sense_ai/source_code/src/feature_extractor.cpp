/**
 * @file feature_extractor.cpp
 * @brief Statistical time-domain vibration and thermal feature extraction implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "feature_extractor.h"
#include <math.h>

void feature_extractor_init(FeatureExtractor_t *extractor) {
    if (!extractor) return;

    for (int i = 0; i < VIBRATION_WINDOW_SIZE; i++) {
        extractor->buffer[i] = 9.80665f; // Baseline 1G
    }
    extractor->head = 0;
    extractor->count = 0;
    extractor->prev_temp_c = 25.0f;
    extractor->prev_temp_time_ms = 0;
}

void feature_extractor_push_sample(FeatureExtractor_t *extractor, float accel_mag) {
    if (!extractor) return;

    extractor->buffer[extractor->head] = accel_mag;
    extractor->head = (extractor->head + 1) % VIBRATION_WINDOW_SIZE;
    if (extractor->count < VIBRATION_WINDOW_SIZE) {
        extractor->count++;
    }
}

bool feature_extractor_compute(FeatureExtractor_t *extractor, float current_temp_c,
                               uint32_t current_time_ms, MachineFeatures_t *out_features) {
    if (!extractor || !out_features) return false;

    if (extractor->count < VIBRATION_WINDOW_SIZE) {
        out_features->window_ready = false;
        return false;
    }

    // 1. Calculate Mean of Acceleration Magnitude
    float sum = 0.0f;
    for (int i = 0; i < VIBRATION_WINDOW_SIZE; i++) {
        sum += extractor->buffer[i];
    }
    float mean = sum / (float)VIBRATION_WINDOW_SIZE;

    // 2. Compute RMS of AC dynamic vibration component (subtract DC gravity)
    float sum_sq_diff = 0.0f;
    float min_val = extractor->buffer[0];
    float max_val = extractor->buffer[0];

    for (int i = 0; i < VIBRATION_WINDOW_SIZE; i++) {
        float val = extractor->buffer[i];
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;

        float ac_component = val - mean;
        sum_sq_diff += (ac_component * ac_component);
    }

    float rms = sqrtf(sum_sq_diff / (float)VIBRATION_WINDOW_SIZE);
    float pk_to_pk = max_val - min_val;
    float peak = pk_to_pk * 0.5f;
    float crest_factor = (rms > 0.001f) ? (peak / rms) : 1.0f;

    // 3. Compute Thermal Drift Derivative
    float dt_min = (current_time_ms - extractor->prev_temp_time_ms) / 60000.0f;
    float temp_rate = 0.0f;
    if (dt_min > 0.01f && extractor->prev_temp_time_ms > 0) {
        temp_rate = (current_temp_c - extractor->prev_temp_c) / dt_min;
    }
    extractor->prev_temp_c = current_temp_c;
    extractor->prev_temp_time_ms = current_time_ms;

    out_features->vibration_rms = rms;
    out_features->peak_to_peak = pk_to_pk;
    out_features->crest_factor = crest_factor;
    out_features->mean_temperature_c = current_temp_c;
    out_features->temp_rate_deg_per_min = temp_rate;
    out_features->window_ready = true;

    return true;
}
