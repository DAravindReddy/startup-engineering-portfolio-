/**
 * @file crack_detector.h
 * @brief Dual-rail optical infrared crack detection module interface.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef CRACK_DETECTOR_H
#define CRACK_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RAIL_STATUS_OK = 0,
    RAIL_CRACK_LEFT,
    RAIL_CRACK_RIGHT,
    RAIL_CRACK_BOTH
} RailCrackStatus_t;

typedef struct {
    uint8_t pin_left;
    uint8_t pin_right;
    bool raw_left_fault;
    bool raw_right_fault;
    uint32_t left_fault_start_ms;
    uint32_t right_fault_start_ms;
    RailCrackStatus_t debounced_status;
} CrackDetector_t;

void crack_detector_init(CrackDetector_t *detector, uint8_t pin_l, uint8_t pin_r);

RailCrackStatus_t crack_detector_update(CrackDetector_t *detector, uint32_t current_time_ms);

#ifdef __cplusplus
}
#endif

#endif // CRACK_DETECTOR_H
