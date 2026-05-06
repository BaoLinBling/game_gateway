#include "middleware/breaker/circuit_breaker.h"

CircuitBreaker::CircuitBreaker(double threshold, int window_ms, int open_ms)
    : threshold_(threshold),
      window_ms_(window_ms),
      open_ms_(open_ms),
      success_(0),
      fail_(0),
      state_(CLOSED) {
    start_ = std::chrono::steady_clock::now();
}

bool CircuitBreaker::allow() {
    checkState();
    return state_.load() != OPEN;
}

void CircuitBreaker::recordSuccess() {
    success_++;
}

void CircuitBreaker::recordFailure() {
    fail_++;
}

void CircuitBreaker::checkState() {
    auto now = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();

    if (elapsed < window_ms_) return;

    int s = success_.load();
    int f = fail_.load();
    int total = s + f;

    if (total == 0) return;

    double rate = (double)f / total;

    if (rate > threshold_) {
        state_ = OPEN;
        start_ = now;
        success_ = 0;
        fail_ = 0;
    } else {
        state_ = CLOSED;
        success_ = 0;
        fail_ = 0;
        start_ = now;
    }
}
