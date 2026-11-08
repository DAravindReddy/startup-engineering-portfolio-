/**
 * @file test_sensor_monitoring.cpp
 * @brief Unit test harness for Multi-Sensor Telemetry Aggregator.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include "sensor_monitor.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>

static void test_initialization() {
    sensor_monitor_t mon;
    sensor_monitor_init(&mon);
    assert(mon.count == 0);
    assert(mon.head == 0);
    assert(mon.total_frames_processed == 0);
    assert(mon.active_alerts == ALERT_NONE);
    std::cout << "[PASS] test_initialization\n";
}

static void test_ring_buffer_and_stats() {
    sensor_monitor_t mon;
    sensor_monitor_init(&mon);

    // Push 4 frames: temperatures 20.0, 22.0, 24.0, 26.0 (mean = 23.0)
    for (int i = 0; i < 4; ++i) {
        sensor_frame_t frame;
        memset(&frame, 0, sizeof(frame));
        frame.timestamp_ms = (uint64_t)(i * 1000);
        frame.temperature_c = 20.0f + (float)(i * 2);
        frame.humidity_pct = 50.0f;
        frame.pressure_hpa = 1013.25f;
        frame.light_lux = 300.0f;
        frame.air_quality_ppm = 50.0f;
        sensor_monitor_push_frame(&mon, &frame);
    }

    assert(mon.count == 4);
    assert(mon.total_frames_processed == 4);

    telemetry_summary_t sum;
    sensor_monitor_compute_stats(&mon, &sum);

    assert(sum.temp_stats.sample_count == 4);
    assert(std::fabs(sum.temp_stats.min_val - 20.0f) < 0.01f);
    assert(std::fabs(sum.temp_stats.max_val - 26.0f) < 0.01f);
    assert(std::fabs(sum.temp_stats.mean_val - 23.0f) < 0.01f);

    // std_dev = sqrt(((20-23)^2 + (22-23)^2 + (24-23)^2 + (26-23)^2) / 4)
    //         = sqrt((9 + 1 + 1 + 9) / 4) = sqrt(20 / 4) = sqrt(5) approx 2.236
    assert(std::fabs(sum.temp_stats.std_dev - 2.236f) < 0.01f);
    std::cout << "[PASS] test_ring_buffer_and_stats\n";
}

static void test_alert_thresholds() {
    sensor_monitor_t mon;
    sensor_monitor_init(&mon);

    // Frame 1: Normal conditions
    sensor_frame_t f1 = {1000ULL, 25.0f, 50.0f, 1013.0f, 500.0f, 50.0f};
    sensor_monitor_push_frame(&mon, &f1);
    uint32_t alerts1 = sensor_monitor_check_alerts(&mon);
    assert(alerts1 == ALERT_NONE);

    // Frame 2: Temperature Warning (40.0C >= 38.0C)
    sensor_frame_t f2 = {2000ULL, 40.0f, 40.0f, 1010.0f, 600.0f, 80.0f};
    sensor_monitor_push_frame(&mon, &f2);
    uint32_t alerts2 = sensor_monitor_check_alerts(&mon);
    assert(alerts2 & ALERT_TEMP_WARNING);
    assert(!(alerts2 & ALERT_TEMP_CRITICAL));

    // Frame 3: Temperature Critical (52.0C >= 50.0C) and Air Warning (250ppm >= 200ppm)
    sensor_frame_t f3 = {3000ULL, 52.0f, 30.0f, 940.0f, 100.0f, 250.0f};
    sensor_monitor_push_frame(&mon, &f3);
    uint32_t alerts3 = sensor_monitor_check_alerts(&mon);
    assert(alerts3 & ALERT_TEMP_CRITICAL);
    assert(alerts3 & ALERT_AIR_WARNING);
    assert(alerts3 & ALERT_PRESSURE_DROP); // 940 <= 950
    std::cout << "[PASS] test_alert_thresholds\n";
}

static void test_json_serialization() {
    sensor_monitor_t mon;
    sensor_monitor_init(&mon);

    sensor_frame_t f = {5000ULL, 28.5f, 55.2f, 1012.3f, 450.0f, 75.4f};
    sensor_monitor_push_frame(&mon, &f);

    char buf[512];
    bool ok = sensor_monitor_format_json(&mon, buf, sizeof(buf));
    assert(ok == true);
    assert(strstr(buf, "\"temp_c\":28.50") != nullptr);
    assert(strstr(buf, "\"humidity_pct\":55.20") != nullptr);
    assert(strstr(buf, "\"pressure_hpa\":1012.30") != nullptr);
    std::cout << "[PASS] test_json_serialization\n";
}

int main() {
    std::cout << "========================================================\n";
    std::cout << " Kernelwise Labs: Sensor Monitoring Unit Tests          \n";
    std::cout << "========================================================\n";

    test_initialization();
    test_ring_buffer_and_stats();
    test_alert_thresholds();
    test_json_serialization();

    std::cout << "========================================================\n";
    std::cout << " ALL UNIT TESTS COMPLETED SUCCESSFULLY (100%)           \n";
    std::cout << "========================================================\n";
    return 0;
}
