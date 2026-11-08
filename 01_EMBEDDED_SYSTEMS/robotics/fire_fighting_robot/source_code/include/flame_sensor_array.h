/**
 * @file flame_sensor_array.h
 * @brief 3-Channel Infrared Flame Detector Array interface.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#ifndef FLAME_SENSOR_ARRAY_H
#define FLAME_SENSOR_ARRAY_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FLAME_NONE = 0,
    FLAME_POS_LEFT,
    FLAME_POS_CENTER,
    FLAME_POS_RIGHT,
    FLAME_POS_MULTIPLE
} FlamePosition_t;

typedef struct {
    uint8_t pin_left;
    uint8_t pin_center;
    uint8_t pin_right;
    bool flame_left;
    bool flame_center;
    bool flame_right;
    FlamePosition_t primary_flame;
} FlameSensorArray_t;

void flame_sensors_init(FlameSensorArray_t *sensors, uint8_t pin_l, uint8_t pin_c, uint8_t pin_r);

FlamePosition_t flame_sensors_sample(FlameSensorArray_t *sensors);

bool flame_is_any_detected(const FlameSensorArray_t *sensors);

#ifdef __cplusplus
}
#endif

#endif // FLAME_SENSOR_ARRAY_H
