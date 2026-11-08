/**
 * @file tinyml_anomaly_detector.cpp
 * @brief Embedded on-device TinyML machine health anomaly scoring implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "tinyml_anomaly_detector.h"
#include <math.h>

// Baseline statistics for healthy machine operation
// Feature 0: Vibration RMS (Mean: 1.8 m/s², StdDev: 0.6 m/s²)
#define BASELINE_RMS_MEAN       1.8f
#define BASELINE_RMS_STD        0.6f

// Feature 1: Peak-to-Peak (Mean: 5.0 m/s², StdDev: 1.5 m/s²)
#define BASELINE_PKPK_MEAN      5.0f
#define BASELINE_PKPK_STD       1.5f

// Feature 2: Temperature (Mean: 40.0 °C, StdDev: 4.0 °C)
#define BASELINE_TEMP_MEAN      40.0f
#define BASELINE_TEMP_STD       4.0f

void tinyml_detector_init(void) {
    // Model initialization
}

void tinyml_detector_predict(const MachineFeatures_t *features, AnomalyInference_t *out_result) {
    if (!features || !out_result) return;

    if (!features->window_ready) {
        out_result->anomaly_score_pct = 0.0f;
        out_result->state = MACHINE_STATE_NORMAL;
        out_result->primary_cause = "Buffering Initial Samples";
        return;
    }

    // Standardized Z-scores
    float z_rms  = (features->vibration_rms - BASELINE_RMS_MEAN) / BASELINE_RMS_STD;
    float z_pkpk = (features->peak_to_peak - BASELINE_PKPK_MEAN) / BASELINE_PKPK_STD;
    float z_temp = (features->mean_temperature_c - BASELINE_TEMP_MEAN) / BASELINE_TEMP_STD;

    // Reject negative Z-scores (lower vibration or cooler temp is healthy)
    if (z_rms < 0.0f)  z_rms = 0.0f;
    if (z_pkpk < 0.0f) z_pkpk = 0.0f;
    if (z_temp < 0.0f) z_temp = 0.0f;

    // Squared normalized anomaly distance
    float distance_sq = (z_rms * z_rms * 1.5f) + (z_pkpk * z_pkpk * 0.8f) + (z_temp * z_temp * 1.2f);

    // Sigmoid mapping to continuous 0.0% to 100.0%
    float score = 100.0f / (1.0f + expf(-0.45f * (distance_sq - 6.0f)));
    if (score < 0.0f) score = 0.0f;
    if (score > 100.0f) score = 100.0f;

    out_result->anomaly_score_pct = score;

    // Classify primary cause
    if (z_rms > 4.0f || z_pkpk > 4.0f) {
        out_result->primary_cause = "Mechanical Bearing Fault / Imbalance";
    } else if (z_temp > 4.0f || features->mean_temperature_c > TEMP_NORMAL_MAX_C) {
        out_result->primary_cause = "Thermal Overheating / Cooling Failure";
    } else if (score >= ANOMALY_SCORE_WARNING_PCT) {
        out_result->primary_cause = "Elevated Vibration Harmonic Strain";
    } else {
        out_result->primary_cause = "Nominal Machine Operation";
    }

    // State classification
    if (score >= ANOMALY_SCORE_CRITICAL_PCT) {
        out_result->state = MACHINE_STATE_CRITICAL_FAULT;
    } else if (score >= ANOMALY_SCORE_WARNING_PCT) {
        out_result->state = MACHINE_STATE_WARNING;
    } else {
        out_result->state = MACHINE_STATE_NORMAL;
    }
}
