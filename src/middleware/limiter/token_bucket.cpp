#include "middleware/limiter/token_bucket.h"

TokenBucket::TokenBucket(int qps, int burst)
    : qps_(qps), burst_(burst), tokens_(burst) {
    last_ = std::chrono::steady_clock::now();
}

void TokenBucket::refill() {
    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_).count();

    int add = (ms * qps_) / 1000;
    if (add > 0) {
        int cur = tokens_.load();
        int new_tokens = std::min(burst_, cur + add);
        tokens_.store(new_tokens);
        last_ = now;
    }
}

bool TokenBucket::allow() {
    std::lock_guard<std::mutex> lock(mtx_);
    refill();

    int cur = tokens_.load();
    if (cur <= 0) return false;

    tokens_.store(cur - 1);
    return true;
}
