/**
 * @file crack_detector.cpp
 * @brief Dual-rail optical infrared crack detection module implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "crack_detector.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
static void pinMode(uint8_t, uint8_t) {}
static int digitalRead(uint8_t) { return 0; /* 0 = Continuous rail reflecting */ }
#define INPUT_PULLUP 2
#define HIGH         1
#define LOW          0
#endif

void crack_detector_init(CrackDetector_t *detector, uint8_t pin_l, uint8_t pin_r) {
    if (!detector) return;

    detector->pin_left = pin_l;
    detector->pin_right = pin_r;
    detector->raw_left_fault = false;
    detector->raw_right_fault = false;
    detector->left_fault_start_ms = 0;
    detector->right_fault_start_ms = 0;
    detector->debounced_status = RAIL_STATUS_OK;

    pinMode(pin_l, INPUT_PULLUP);
    pinMode(pin_r, INPUT_PULLUP);
}

RailCrackStatus_t crack_detector_update(CrackDetector_t *detector, uint32_t current_time_ms) {
    if (!detector) return RAIL_STATUS_OK;

    // Optical reflectance: when rail has a gap/crack, reflection is lost (digital HIGH on active-low comparator)
    bool left_tripped = (digitalRead(detector->pin_left) == HIGH);
    bool right_tripped = (digitalRead(detector->pin_right) == HIGH);

    // Left Rail Debounce
    if (left_tripped) {
        if (!detector->raw_left_fault) {
            detector->raw_left_fault = true;
            detector->left_fault_start_ms = current_time_ms;
        }
    } else {
        detector->raw_left_fault = false;
        detector->left_fault_start_ms = 0;
    }

    // Right Rail Debounce
    if (right_tripped) {
        if (!detector->raw_right_fault) {
            detector->raw_right_fault = true;
            detector->right_fault_start_ms = current_time_ms;
        }
    } else {
        detector->raw_right_fault = false;
        detector->right_fault_start_ms = 0;
    }

    bool left_confirmed = detector->raw_left_fault &&
                          ((current_time_ms - detector->left_fault_start_ms) >= CRACK_DEBOUNCE_MS);
    bool right_confirmed = detector->raw_right_fault &&
                           ((current_time_ms - detector->right_fault_start_ms) >= CRACK_DEBOUNCE_MS);

    if (left_confirmed && right_confirmed) {
        detector->debounced_status = RAIL_CRACK_BOTH;
    } else if (left_confirmed) {
        detector->debounced_status = RAIL_CRACK_LEFT;
    } else if (right_confirmed) {
        detector->debounced_status = RAIL_CRACK_RIGHT;
    } else {
        detector->debounced_status = RAIL_STATUS_OK;
    }

    return detector->debounced_status;
}
