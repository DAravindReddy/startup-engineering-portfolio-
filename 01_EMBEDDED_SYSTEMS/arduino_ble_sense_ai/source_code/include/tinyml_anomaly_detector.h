/**
 * @file tinyml_anomaly_detector.h
 * @brief Embedded on-device TinyML machine health anomaly scoring engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef TINYML_ANOMALY_DETECTOR_H
#define TINYML_ANOMALY_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "feature_extractor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MACHINE_STATE_NORMAL = 0,
    MACHINE_STATE_WARNING,
    MACHINE_STATE_CRITICAL_FAULT
} MachineState_t;

typedef struct {
    float anomaly_score_pct;      // 0.0% (Optimal) to 100.0% (Critical Failure)
    MachineState_t state;         // Discrete operational category
    const char *primary_cause;    // Diagnosed fault (e.g. Bearing Wear, Overheating)
} AnomalyInference_t;

void tinyml_detector_init(void);

void tinyml_detector_predict(const MachineFeatures_t *features, AnomalyInference_t *out_result);

#ifdef __cplusplus
}
#endif

#endif // TINYML_ANOMALY_DETECTOR_H
