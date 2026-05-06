#pragma once
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace coroutine_cpp20 {

class AdvancedScheduler {
public:
    AdvancedScheduler(int thread_num = std::thread::hardware_concurrency());
    ~AdvancedScheduler();

    void schedule(std::function<void()> fn);
    void run();
    void stop();

    static AdvancedScheduler& instance();

private:
    void workerLoop();

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> running_{true};
};

}
