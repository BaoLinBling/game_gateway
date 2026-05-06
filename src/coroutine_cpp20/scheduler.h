#pragma once
#include <queue>
#include <coroutine>

class Scheduler {
public:
    static Scheduler& Instance() {
        static Scheduler s;
        return s;
    }

    void Schedule(std::coroutine_handle<> h) {
        queue_.push(h);
    }

    void Run() {
        while (!queue_.empty()) {
            auto h = queue_.front();
            queue_.pop();
            h.resume();
        }
    }

private:
    std::queue<std::coroutine_handle<>> queue_;
};
