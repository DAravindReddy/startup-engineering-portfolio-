/**
 * @file test_tinyml_pipeline.cpp
 * @brief Unit tests for Arduino BLE Sense TinyML Feature Extraction & Anomaly Inference.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <cassert>
#include <cstdio>
#include <cmath>

#include "../../source_code/include/config.h"
#include "../../source_code/include/sensor_manager.h"
#include "../../source_code/include/feature_extractor.h"
#include "../../source_code/include/tinyml_anomaly_detector.h"

static void test_feature_extractor_math() {
    printf("[TEST 1] Verifying feature extractor RMS and peak-to-peak calculations...\n");
    FeatureExtractor_t extractor;
    feature_extractor_init(&extractor);

    // Push 64 synthetic alternating samples: 9.81 + 2.0 and 9.81 - 2.0 (amplitude = 2.0 m/s²)
    for (int i = 0; i < VIBRATION_WINDOW_SIZE; i++) {
        float sample = 9.80665f + ((i % 2 == 0) ? 2.0f : -2.0f);
        feature_extractor_push_sample(&extractor, sample);
    }

    MachineFeatures_t feat;
    bool ok = feature_extractor_compute(&extractor, 40.0f, 1000, &feat);
    assert(ok);
    assert(feat.window_ready);

    // Theoretical RMS of square wave with amplitude A = 2.0 is 2.0
    assert(fabsf(feat.vibration_rms - 2.0f) < 0.1f);
    assert(fabsf(feat.peak_to_peak - 4.0f) < 0.1f);
    printf("  --> PASS: Feature extractor RMS (%.2f m/s²) and Pk-Pk (%.2f m/s²) verified.\n",
           feat.vibration_rms, feat.peak_to_peak);
}

static void test_tinyml_nominal_prediction() {
    printf("[TEST 2] Verifying TinyML anomaly scoring on nominal machine features...\n");
    MachineFeatures_t feat;
    feat.vibration_rms = 1.8f;      // Baseline mean
    feat.peak_to_peak = 5.0f;       // Baseline mean
    feat.mean_temperature_c = 40.0f;// Baseline mean
    feat.window_ready = true;

    AnomalyInference_t result;
    tinyml_detector_predict(&feat, &result);

    assert(result.state == MACHINE_STATE_NORMAL);
    assert(result.anomaly_score_pct < 30.0f);
    printf("  --> PASS: Baseline operating health verified (Score: %.1f%%, State: Normal).\n",
           result.anomaly_score_pct);
}

static void test_tinyml_vibration_bearing_fault() {
    printf("[TEST 3] Verifying TinyML anomaly detection on acute mechanical vibration...\n");
    MachineFeatures_t feat;
    feat.vibration_rms = 8.5f;       // Severe bearing chatter (baseline 1.8)
    feat.peak_to_peak = 25.0f;      // Extreme shock peaks
    feat.mean_temperature_c = 45.0f;
    feat.window_ready = true;

    AnomalyInference_t result;
    tinyml_detector_predict(&feat, &result);

    assert(result.state == MACHINE_STATE_CRITICAL_FAULT);
    assert(result.anomaly_score_pct >= ANOMALY_SCORE_CRITICAL_PCT);
    printf("  --> PASS: Bearing fault isolated! Score: %.1f%%, Cause: %s\n",
           result.anomaly_score_pct, result.primary_cause);
}

static void test_tinyml_thermal_overheating() {
    printf("[TEST 4] Verifying TinyML anomaly detection on thermal overheating...\n");
    MachineFeatures_t feat;
    feat.vibration_rms = 1.9f;       // Normal vibration
    feat.peak_to_peak = 5.2f;
    feat.mean_temperature_c = 86.0f; // Critical thermal runaway (> 80°C)
    feat.window_ready = true;

    AnomalyInference_t result;
    tinyml_detector_predict(&feat, &result);

    assert(result.state == MACHINE_STATE_CRITICAL_FAULT);
    assert(result.anomaly_score_pct >= ANOMALY_SCORE_CRITICAL_PCT);
    printf("  --> PASS: Thermal fault isolated! Score: %.1f%%, Cause: %s\n",
           result.anomaly_score_pct, result.primary_cause);
}

int main() {
    printf("====================================================\n");
    printf(" Arduino BLE Sense TinyML Pipeline Unit Tests       \n");
    printf("====================================================\n");

    test_feature_extractor_math();
    test_tinyml_nominal_prediction();
    test_tinyml_vibration_bearing_fault();
    test_tinyml_thermal_overheating();

    printf("\n>>> ALL 4 TEST SUITES PASSED CLEANLY (100%% SUCCESS) <<<\n");
    return 0;
}
