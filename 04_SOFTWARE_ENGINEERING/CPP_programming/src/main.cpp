/**
 * @file main.cpp
 * @brief Modern C++17 Demonstration: RAII, Move Semantics, Concurrency, and Lambdas.
 * @author Aravind Reddy (Kernelwise Labs)
 * @license MIT
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include "../include/thread_pool.hpp"

// RAII Buffer Example
class ScopedResource {
public:
    explicit ScopedResource(size_t size) : size_(size), data_(new int[size]) {
        std::cout << "[RAII] Allocated resource of size " << size_ << " elements.\n";
    }

    // Rule of Five: Delete copy, implement move
    ScopedResource(const ScopedResource&) = delete;
    ScopedResource& operator=(const ScopedResource&) = delete;

    ScopedResource(ScopedResource&& other) noexcept 
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "[Move] Transferred ownership via move semantics.\n";
    }

    ScopedResource& operator=(ScopedResource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~ScopedResource() {
        if (data_) {
            std::cout << "[RAII] Freed resource of size " << size_ << ".\n";
            delete[] data_;
        }
    }

    size_t size() const { return size_; }

private:
    size_t size_;
    int* data_;
};

int main() {
    std::cout << "====================================================\n";
    std::cout << " Modern C++17 Core Systems Programming Suite       \n";
    std::cout << "====================================================\n";

    // 1. Move Semantics & RAII
    {
        ScopedResource res1(1024);
        ScopedResource res2 = std::move(res1);
        std::cout << "Resource 2 active size: " << res2.size() << "\n";
    }

    // 2. Concurrency via ThreadPool
    std::cout << "\n[Concurrency] Dispatching 6 parallel asynchronous tasks...\n";
    kw::ThreadPool pool(4);
    std::vector<std::future<int>> results;

    for (int i = 1; i <= 6; ++i) {
        results.emplace_back(pool.enqueue([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            return i * i;
        }));
    }

    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "Task " << (i + 1) << " computed square: " << results[i].get() << "\n";
    }

    // 3. STL Algorithms & Lambdas
    std::vector<int> numbers(10);
    std::iota(numbers.begin(), numbers.end(), 1);

    auto sum_even = std::accumulate(numbers.begin(), numbers.end(), 0, [](int acc, int val) {
        return (val % 2 == 0) ? (acc + val) : acc;
    });

    std::cout << "\n[STL] Sum of even numbers 1..10: " << sum_even << " (Expected: 30)\n";
    std::cout << ">>> Modern C++17 Suite Executed Successfully <<<\n";
    return 0;
}
