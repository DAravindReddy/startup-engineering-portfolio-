/**
 * @file digital_watch.h
 * @brief High-precision multi-threaded digital watch core engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#ifndef DIGITAL_WATCH_H
#define DIGITAL_WATCH_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define MAX_STOPWATCH_LAPS 16
#define WATCH_BUFFER_SIZE  128

typedef enum {
    WATCH_MODE_CLOCK = 0,
    WATCH_MODE_STOPWATCH,
    WATCH_MODE_TIMER,
    WATCH_MODE_ALARM
} watch_mode_t;

typedef enum {
    STOPWATCH_STOPPED = 0,
    STOPWATCH_RUNNING,
    STOPWATCH_PAUSED
} stopwatch_status_t;

typedef struct {
    uint32_t lap_number;
    uint64_t lap_time_ms;
    uint64_t split_time_ms;
} stopwatch_lap_t;

typedef struct {
    stopwatch_status_t status;
    uint64_t start_time_ms;
    uint64_t accumulated_ms;
    uint32_t lap_count;
    stopwatch_lap_t laps[MAX_STOPWATCH_LAPS];
} stopwatch_t;

typedef struct {
    bool active;
    bool triggered;
    uint32_t initial_seconds;
    uint32_t remaining_seconds;
    uint64_t last_tick_ms;
} timer_t;

typedef struct {
    bool enabled;
    bool triggered;
    uint8_t hour;
    uint8_t minute;
} alarm_t;

typedef struct {
    watch_mode_t current_mode;
    struct tm current_time;
    uint64_t current_timestamp_ms;
    stopwatch_t stopwatch;
    timer_t timer;
    alarm_t alarm;
    bool running;
} watch_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize digital watch state structure.
 * @param state Pointer to watch state structure.
 */
void watch_init(watch_state_t *state);

/**
 * @brief Retrieve monotonic millisecond timestamp.
 * @return Current timestamp in milliseconds.
 */
uint64_t watch_get_monotonic_ms(void);

/**
 * @brief Update current local clock time.
 * @param state Pointer to watch state.
 * @param epoch_override Optional mock epoch timestamp (0 for live host time).
 */
void watch_update_clock(watch_state_t *state, time_t epoch_override);

/**
 * @brief Switch active watch display mode.
 * @param state Pointer to watch state.
 * @param new_mode Target display mode.
 */
void watch_set_mode(watch_state_t *state, watch_mode_t new_mode);

/* --- Stopwatch Subsystem --- */
void stopwatch_start(watch_state_t *state, uint64_t now_ms);
void stopwatch_pause(watch_state_t *state, uint64_t now_ms);
void stopwatch_resume(watch_state_t *state, uint64_t now_ms);
void stopwatch_reset(watch_state_t *state);
bool stopwatch_record_lap(watch_state_t *state, uint64_t now_ms);
uint64_t stopwatch_get_elapsed_ms(const watch_state_t *state, uint64_t now_ms);

/* --- Countdown Timer Subsystem --- */
void timer_start(watch_state_t *state, uint32_t seconds, uint64_t now_ms);
void timer_stop(watch_state_t *state);
void timer_tick(watch_state_t *state, uint64_t now_ms);
void timer_reset(watch_state_t *state);

/* --- Alarm Subsystem --- */
void alarm_set(watch_state_t *state, uint8_t hour, uint8_t minute);
void alarm_toggle(watch_state_t *state, bool enable);
bool alarm_evaluate(watch_state_t *state);
void alarm_acknowledge(watch_state_t *state);

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_WATCH_H */
