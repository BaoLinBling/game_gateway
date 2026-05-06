#pragma once
#include <atomic>
#include <chrono>
#include <mutex>

class TokenBucket {
public:
    TokenBucket(int qps, int burst);

    bool allow();

private:
    void refill();

private:
    int qps_;
    int burst_;

    std::atomic<int> tokens_;
    std::chrono::steady_clock::time_point last_;

    std::mutex mtx_;
};
