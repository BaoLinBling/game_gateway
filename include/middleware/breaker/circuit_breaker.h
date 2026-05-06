#pragma once
#include <atomic>
#include <chrono>
#include <mutex>

class CircuitBreaker {
public:
    CircuitBreaker(double threshold, int window_ms, int open_ms);

    bool allow();
    void recordSuccess();
    void recordFailure();

private:
    void checkState();

private:
    double threshold_;
    int window_ms_;
    int open_ms_;

    std::atomic<int> success_;
    std::atomic<int> fail_;

    std::chrono::steady_clock::time_point start_;

    enum State { CLOSED, OPEN, HALF };
    std::atomic<State> state_;

    std::mutex mtx_;
};
