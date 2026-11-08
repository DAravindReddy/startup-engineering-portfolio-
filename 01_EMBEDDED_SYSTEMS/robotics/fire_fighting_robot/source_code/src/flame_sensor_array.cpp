/**
 * @file flame_sensor_array.cpp
 * @brief 3-Channel Infrared Flame Detector Array implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include "flame_sensor_array.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
static void pinMode(uint8_t, uint8_t) {}
static int digitalRead(uint8_t) { return 1; /* 1 = No flame in active-LOW modules */ }
#define INPUT_PULLUP 2
#define LOW          0
#define HIGH         1
#endif

void flame_sensors_init(FlameSensorArray_t *sensors, uint8_t pin_l, uint8_t pin_c, uint8_t pin_r) {
    if (!sensors) return;

    sensors->pin_left = pin_l;
    sensors->pin_center = pin_c;
    sensors->pin_right = pin_r;
    sensors->flame_left = false;
    sensors->flame_center = false;
    sensors->flame_right = false;
    sensors->primary_flame = FLAME_NONE;

    pinMode(pin_l, INPUT_PULLUP);
    pinMode(pin_c, INPUT_PULLUP);
    pinMode(pin_r, INPUT_PULLUP);
}

FlamePosition_t flame_sensors_sample(FlameSensorArray_t *sensors) {
    if (!sensors) return FLAME_NONE;

    // Optical flame modules with LM393 comparators pull LOW when IR flame is detected
    sensors->flame_left = (digitalRead(sensors->pin_left) == LOW);
    sensors->flame_center = (digitalRead(sensors->pin_center) == LOW);
    sensors->flame_right = (digitalRead(sensors->pin_right) == LOW);

    uint8_t count = (sensors->flame_left ? 1 : 0) +
                    (sensors->flame_center ? 1 : 0) +
                    (sensors->flame_right ? 1 : 0);

    if (count == 0) {
        sensors->primary_flame = FLAME_NONE;
    } else if (count > 1) {
        sensors->primary_flame = FLAME_POS_MULTIPLE;
    } else if (sensors->flame_center) {
        sensors->primary_flame = FLAME_POS_CENTER;
    } else if (sensors->flame_left) {
        sensors->primary_flame = FLAME_POS_LEFT;
    } else {
        sensors->primary_flame = FLAME_POS_RIGHT;
    }

    return sensors->primary_flame;
}

bool flame_is_any_detected(const FlameSensorArray_t *sensors) {
    return sensors && (sensors->flame_left || sensors->flame_center || sensors->flame_right);
}
