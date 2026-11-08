/**
 * @file feature_extractor.h
 * @brief Statistical time-domain vibration and thermal feature extraction.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float vibration_rms;        // Root Mean Square vibration energy (m/s²)
    float peak_to_peak;         // Max - Min acceleration amplitude (m/s²)
    float crest_factor;         // Peak / RMS ratio (impact indicator)
    float mean_temperature_c;   // Operating temperature (°C)
    float temp_rate_deg_per_min;// Thermal drift derivative (°C/min)
    bool window_ready;          // True when full N=64 sample buffer filled
} MachineFeatures_t;

typedef struct {
    float buffer[VIBRATION_WINDOW_SIZE];
    uint16_t head;
    uint16_t count;
    float prev_temp_c;
    uint32_t prev_temp_time_ms;
} FeatureExtractor_t;

void feature_extractor_init(FeatureExtractor_t *extractor);

void feature_extractor_push_sample(FeatureExtractor_t *extractor, float accel_mag);

bool feature_extractor_compute(FeatureExtractor_t *extractor, float current_temp_c,
                               uint32_t current_time_ms, MachineFeatures_t *out_features);

#ifdef __cplusplus
}
#endif

#endif // FEATURE_EXTRACTOR_H
