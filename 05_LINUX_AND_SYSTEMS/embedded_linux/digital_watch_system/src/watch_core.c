/**
 * @file watch_core.c
 * @brief Core timekeeping, stopwatch, timer, and alarm implementation.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "digital_watch.h"
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

void watch_init(watch_state_t *state) {
    if (!state) return;
    memset(state, 0, sizeof(watch_state_t));
    state->current_mode = WATCH_MODE_CLOCK;
    state->running = true;
    state->alarm.enabled = false;
    state->alarm.triggered = false;
}

uint64_t watch_get_monotonic_ms(void) {
#if defined(_WIN32)
    static LARGE_INTEGER freq;
    static int initialized = 0;
    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (uint64_t)((counter.QuadPart * 1000ULL) / freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((uint64_t)ts.tv_sec * 1000ULL) + ((uint64_t)ts.tv_nsec / 1000000ULL);
#endif
}

void watch_update_clock(watch_state_t *state, time_t epoch_override) {
    if (!state) return;
    time_t raw_time = epoch_override ? epoch_override : time(NULL);
#if defined(_WIN32)
    localtime_s(&state->current_time, &raw_time);
#else
    localtime_r(&raw_time, &state->current_time);
#endif
    state->current_timestamp_ms = watch_get_monotonic_ms();
}

void watch_set_mode(watch_state_t *state, watch_mode_t new_mode) {
    if (!state) return;
    state->current_mode = new_mode;
}

/* --- Stopwatch Subsystem --- */
void stopwatch_start(watch_state_t *state, uint64_t now_ms) {
    if (!state) return;
    if (state->stopwatch.status == STOPWATCH_STOPPED) {
        state->stopwatch.start_time_ms = now_ms;
        state->stopwatch.accumulated_ms = 0;
        state->stopwatch.lap_count = 0;
        state->stopwatch.status = STOPWATCH_RUNNING;
    } else if (state->stopwatch.status == STOPWATCH_PAUSED) {
        stopwatch_resume(state, now_ms);
    }
}

void stopwatch_pause(watch_state_t *state, uint64_t now_ms) {
    if (!state || state->stopwatch.status != STOPWATCH_RUNNING) return;
    state->stopwatch.accumulated_ms += (now_ms - state->stopwatch.start_time_ms);
    state->stopwatch.status = STOPWATCH_PAUSED;
}

void stopwatch_resume(watch_state_t *state, uint64_t now_ms) {
    if (!state || state->stopwatch.status != STOPWATCH_PAUSED) return;
    state->stopwatch.start_time_ms = now_ms;
    state->stopwatch.status = STOPWATCH_RUNNING;
}

void stopwatch_reset(watch_state_t *state) {
    if (!state) return;
    state->stopwatch.status = STOPWATCH_STOPPED;
    state->stopwatch.accumulated_ms = 0;
    state->stopwatch.start_time_ms = 0;
    state->stopwatch.lap_count = 0;
    memset(state->stopwatch.laps, 0, sizeof(state->stopwatch.laps));
}

bool stopwatch_record_lap(watch_state_t *state, uint64_t now_ms) {
    if (!state || state->stopwatch.status != STOPWATCH_RUNNING) return false;
    if (state->stopwatch.lap_count >= MAX_STOPWATCH_LAPS) return false;

    uint64_t total_elapsed = stopwatch_get_elapsed_ms(state, now_ms);
    uint64_t prev_lap_split = 0;
    if (state->stopwatch.lap_count > 0) {
        prev_lap_split = state->stopwatch.laps[state->stopwatch.lap_count - 1].split_time_ms;
    }

    uint32_t idx = state->stopwatch.lap_count;
    state->stopwatch.laps[idx].lap_number = idx + 1;
    state->stopwatch.laps[idx].split_time_ms = total_elapsed;
    state->stopwatch.laps[idx].lap_time_ms = total_elapsed - prev_lap_split;
    state->stopwatch.lap_count++;
    return true;
}

uint64_t stopwatch_get_elapsed_ms(const watch_state_t *state, uint64_t now_ms) {
    if (!state) return 0;
    if (state->stopwatch.status == STOPWATCH_RUNNING) {
        return state->stopwatch.accumulated_ms + (now_ms - state->stopwatch.start_time_ms);
    }
    return state->stopwatch.accumulated_ms;
}

/* --- Countdown Timer Subsystem --- */
void timer_start(watch_state_t *state, uint32_t seconds, uint64_t now_ms) {
    if (!state || seconds == 0) return;
    state->timer.active = true;
    state->timer.triggered = false;
    state->timer.initial_seconds = seconds;
    state->timer.remaining_seconds = seconds;
    state->timer.last_tick_ms = now_ms;
}

void timer_stop(watch_state_t *state) {
    if (!state) return;
    state->timer.active = false;
}

void timer_tick(watch_state_t *state, uint64_t now_ms) {
    if (!state || !state->timer.active || state->timer.triggered) return;
    if (now_ms - state->timer.last_tick_ms >= 1000ULL) {
        uint32_t elapsed_seconds = (uint32_t)((now_ms - state->timer.last_tick_ms) / 1000ULL);
        state->timer.last_tick_ms += ((uint64_t)elapsed_seconds * 1000ULL);

        if (state->timer.remaining_seconds <= elapsed_seconds) {
            state->timer.remaining_seconds = 0;
            state->timer.active = false;
            state->timer.triggered = true;
        } else {
            state->timer.remaining_seconds -= elapsed_seconds;
        }
    }
}

void timer_reset(watch_state_t *state) {
    if (!state) return;
    state->timer.active = false;
    state->timer.triggered = false;
    state->timer.remaining_seconds = state->timer.initial_seconds;
}

/* --- Alarm Subsystem --- */
void alarm_set(watch_state_t *state, uint8_t hour, uint8_t minute) {
    if (!state) return;
    state->alarm.hour = hour % 24;
    state->alarm.minute = minute % 60;
    state->alarm.enabled = true;
    state->alarm.triggered = false;
}

void alarm_toggle(watch_state_t *state, bool enable) {
    if (!state) return;
    state->alarm.enabled = enable;
    if (!enable) {
        state->alarm.triggered = false;
    }
}

bool alarm_evaluate(watch_state_t *state) {
    if (!state || !state->alarm.enabled) return false;
    if (!state->alarm.triggered &&
        state->current_time.tm_hour == state->alarm.hour &&
        state->current_time.tm_min == state->alarm.minute) {
        state->alarm.triggered = true;
        return true;
    }
    return state->alarm.triggered;
}

void alarm_acknowledge(watch_state_t *state) {
    if (!state) return;
    state->alarm.triggered = false;
}
