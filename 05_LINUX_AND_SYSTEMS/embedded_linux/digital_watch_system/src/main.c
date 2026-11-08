/**
 * @file main.c
 * @brief Main entrypoint for Embedded Linux Digital Watch Application.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "digital_watch.h"
#include <stdio.h>
#include <string.h>

void watch_render_banner(const watch_state_t *state, char *buffer, size_t max_len);
void watch_render_stopwatch(const watch_state_t *state, uint64_t now_ms, char *buffer, size_t max_len);

int main(int argc, char *argv[]) {
    printf("[Kernelwise Labs] Initializing Embedded Linux Digital Watch System...\n");
    watch_state_t watch;
    watch_init(&watch);
    watch_update_clock(&watch, 0);

    char buffer[1024];
    watch_render_banner(&watch, buffer, sizeof(buffer));
    printf("%s", buffer);

    if (argc > 1 && strcmp(argv[1], "--demo") == 0) {
        printf("\n--- Running Automated Subsystem Verification Demo ---\n");
        uint64_t t0 = 100000ULL;
        stopwatch_start(&watch, t0);
        printf("[DEMO] Stopwatch started at T=0\n");

        uint64_t t1 = t0 + 1540ULL;
        stopwatch_record_lap(&watch, t1);
        printf("[DEMO] Recorded Lap 1 at +1540ms\n");

        uint64_t t2 = t1 + 2310ULL;
        stopwatch_record_lap(&watch, t2);
        printf("[DEMO] Recorded Lap 2 at +3850ms\n");

        stopwatch_pause(&watch, t2);
        watch_render_stopwatch(&watch, t2, buffer, sizeof(buffer));
        printf("%s", buffer);

        printf("\n[DEMO] Testing Countdown Timer (5 seconds)...\n");
        timer_start(&watch, 5, t0);
        timer_tick(&watch, t0 + 2000ULL);
        printf("Timer tick +2s: Remaining = %u s, Active = %d\n", watch.timer.remaining_seconds, watch.timer.active);
        timer_tick(&watch, t0 + 5500ULL);
        printf("Timer tick +5.5s: Remaining = %u s, Triggered = %d\n", watch.timer.remaining_seconds, watch.timer.triggered);

        printf("\n[DEMO] Testing Alarm Subsystem...\n");
        alarm_set(&watch, watch.current_time.tm_hour, watch.current_time.tm_min);
        bool triggered = alarm_evaluate(&watch);
        printf("Alarm evaluate (Hour=%02d, Min=%02d): Triggered = %s\n",
               watch.alarm.hour, watch.alarm.minute, triggered ? "YES" : "NO");

        printf("\n[Kernelwise Labs] All watch subsystems successfully verified.\n");
        return 0;
    }

    printf("Run with '--demo' to execute automated verification suite.\n");
    return 0;
}
