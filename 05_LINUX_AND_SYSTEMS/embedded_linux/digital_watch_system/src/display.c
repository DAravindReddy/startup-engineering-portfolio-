/**
 * @file display.c
 * @brief ANSI terminal dashboard rendering engine for embedded digital watch.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "digital_watch.h"
#include <stdio.h>
#include <inttypes.h>

void watch_render_banner(const watch_state_t *state, char *buffer, size_t max_len) {
    if (!state || !buffer || max_len == 0) return;

    const char *mode_str = "CLOCK";
    switch (state->current_mode) {
        case WATCH_MODE_CLOCK: mode_str = "CLOCK"; break;
        case WATCH_MODE_STOPWATCH: mode_str = "STOPWATCH"; break;
        case WATCH_MODE_TIMER: mode_str = "TIMER"; break;
        case WATCH_MODE_ALARM: mode_str = "ALARM"; break;
    }

    snprintf(buffer, max_len,
             "=========================================\n"
             "      EMBEDDED DIGITAL WATCH (ARM/x86)    \n"
             "      Mode: [%s] | Alarm: [%s]            \n"
             "=========================================\n"
             " Current Time: %02d:%02d:%02d\n"
             " Date:         %04d-%02d-%02d\n"
             "-----------------------------------------\n",
             mode_str,
             state->alarm.enabled ? (state->alarm.triggered ? "RINGING!" : "ACTIVE") : "OFF",
             state->current_time.tm_hour,
             state->current_time.tm_min,
             state->current_time.tm_sec,
             state->current_time.tm_year + 1900,
             state->current_time.tm_mon + 1,
             state->current_time.tm_mday);
}

void watch_render_stopwatch(const watch_state_t *state, uint64_t now_ms, char *buffer, size_t max_len) {
    if (!state || !buffer || max_len == 0) return;

    uint64_t elapsed_ms = stopwatch_get_elapsed_ms(state, now_ms);
    uint32_t min = (uint32_t)(elapsed_ms / 60000ULL);
    uint32_t sec = (uint32_t)((elapsed_ms % 60000ULL) / 1000ULL);
    uint32_t ms  = (uint32_t)(elapsed_ms % 1000ULL);

    const char *status_str = (state->stopwatch.status == STOPWATCH_RUNNING) ? "RUNNING" :
                             (state->stopwatch.status == STOPWATCH_PAUSED) ? "PAUSED" : "STOPPED";

    int offset = snprintf(buffer, max_len,
                          " [STOPWATCH] Status: %s\n"
                          " Elapsed: %02u:%02u.%03u\n"
                          " Laps Recorded: %u\n",
                          status_str, min, sec, ms, state->stopwatch.lap_count);

    if (offset > 0 && (size_t)offset < max_len) {
        for (uint32_t i = 0; i < state->stopwatch.lap_count && (size_t)offset < max_len; ++i) {
            uint32_t l_sec = (uint32_t)(state->stopwatch.laps[i].lap_time_ms / 1000ULL);
            uint32_t l_ms  = (uint32_t)(state->stopwatch.laps[i].lap_time_ms % 1000ULL);
            int written = snprintf(buffer + offset, max_len - (size_t)offset,
                                   "   Lap %02u: %02u.%03u (Split: %" PRIu64 " ms)\n",
                                   state->stopwatch.laps[i].lap_number,
                                   l_sec, l_ms,
                                   state->stopwatch.laps[i].split_time_ms);
            if (written > 0) offset += written;
        }
    }
}
