/**
 * @file test_watch_core.c
 * @brief Unit test suite for Embedded Linux Digital Watch Core Engine.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "digital_watch.h"
#include <stdio.h>
#include <assert.h>

static void test_initialization(void) {
    watch_state_t w;
    watch_init(&w);
    assert(w.current_mode == WATCH_MODE_CLOCK);
    assert(w.running == true);
    assert(w.stopwatch.status == STOPWATCH_STOPPED);
    assert(w.stopwatch.lap_count == 0);
    assert(w.timer.active == false);
    assert(w.alarm.enabled == false);
    printf("[PASS] test_initialization\n");
}

static void test_stopwatch_laps(void) {
    watch_state_t w;
    watch_init(&w);

    uint64_t t0 = 10000ULL;
    stopwatch_start(&w, t0);
    assert(w.stopwatch.status == STOPWATCH_RUNNING);

    // Lap 1 at +2500 ms
    assert(stopwatch_record_lap(&w, t0 + 2500ULL) == true);
    assert(w.stopwatch.lap_count == 1);
    assert(w.stopwatch.laps[0].lap_time_ms == 2500ULL);
    assert(w.stopwatch.laps[0].split_time_ms == 2500ULL);

    // Lap 2 at +6000 ms (lap time = 3500ms)
    assert(stopwatch_record_lap(&w, t0 + 6000ULL) == true);
    assert(w.stopwatch.lap_count == 2);
    assert(w.stopwatch.laps[1].lap_time_ms == 3500ULL);
    assert(w.stopwatch.laps[1].split_time_ms == 6000ULL);

    // Pause at +8000 ms
    stopwatch_pause(&w, t0 + 8000ULL);
    assert(w.stopwatch.status == STOPWATCH_PAUSED);
    assert(stopwatch_get_elapsed_ms(&w, t0 + 12000ULL) == 8000ULL);

    // Resume at +15000 ms and check elapsed at +17000 ms (total = 8000 + 2000 = 10000ms)
    stopwatch_resume(&w, t0 + 15000ULL);
    assert(w.stopwatch.status == STOPWATCH_RUNNING);
    assert(stopwatch_get_elapsed_ms(&w, t0 + 17000ULL) == 10000ULL);

    // Reset
    stopwatch_reset(&w);
    assert(w.stopwatch.status == STOPWATCH_STOPPED);
    assert(w.stopwatch.lap_count == 0);
    printf("[PASS] test_stopwatch_laps\n");
}

static void test_countdown_timer(void) {
    watch_state_t w;
    watch_init(&w);

    uint64_t t0 = 50000ULL;
    timer_start(&w, 10, t0);
    assert(w.timer.active == true);
    assert(w.timer.remaining_seconds == 10);
    assert(w.timer.triggered == false);

    // Tick +3s
    timer_tick(&w, t0 + 3100ULL);
    assert(w.timer.active == true);
    assert(w.timer.remaining_seconds == 7);

    // Tick +10s (total elapsed >= 10s)
    timer_tick(&w, t0 + 10500ULL);
    assert(w.timer.active == false);
    assert(w.timer.remaining_seconds == 0);
    assert(w.timer.triggered == true);

    // Reset
    timer_reset(&w);
    assert(w.timer.active == false);
    assert(w.timer.triggered == false);
    assert(w.timer.remaining_seconds == 10);
    printf("[PASS] test_countdown_timer\n");
}

static void test_alarm_subsystem(void) {
    watch_state_t w;
    watch_init(&w);

    // Preset time: 14:30:00 (1577889000 approx)
    time_t mock_epoch = 1609459200; // 2021-01-01 00:00:00 UTC
    watch_update_clock(&w, mock_epoch);

    alarm_set(&w, w.current_time.tm_hour, w.current_time.tm_min);
    assert(w.alarm.enabled == true);
    assert(alarm_evaluate(&w) == true);
    assert(w.alarm.triggered == true);

    alarm_acknowledge(&w);
    assert(w.alarm.triggered == false);
    printf("[PASS] test_alarm_subsystem\n");
}

int main(void) {
    printf("==================================================\n");
    printf(" Kernelwise Labs: Digital Watch Core Unit Tests   \n");
    printf("==================================================\n");

    test_initialization();
    test_stopwatch_laps();
    test_countdown_timer();
    test_alarm_subsystem();

    printf("==================================================\n");
    printf(" ALL UNIT TESTS PASSED SUCCESSFULLY (100%%)       \n");
    printf("==================================================\n");
    return 0;
}
