#include "coroutine_cpp20/advanced_scheduler.h"

namespace coroutine_cpp20 {

AdvancedScheduler::AdvancedScheduler(int thread_num) {
    for (int i = 0; i < thread_num; ++i) {
        workers_.emplace_back([this]() { workerLoop(); });
    }
}

AdvancedScheduler::~AdvancedScheduler() {
    stop();
}

void AdvancedScheduler::schedule(std::function<void()> fn) {
    {
        std::lock_guard lock(mtx_);
        tasks_.push(std::move(fn));
    }
    cv_.notify_one();
}

void AdvancedScheduler::workerLoop() {
    while (running_) {
        std::function<void()> task;

        {
            std::unique_lock lock(mtx_);
            cv_.wait(lock, [&]() {
                return !tasks_.empty() || !running_;
            });

            if (!running_) return;

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        task();
    }
}

void AdvancedScheduler::stop() {
    running_ = false;
    cv_.notify_all();

    for (auto& t : workers_) {
        if (t.joinable()) t.join();
    }
}

AdvancedScheduler& AdvancedScheduler::instance() {
    static AdvancedScheduler s;
    return s;
}

}
