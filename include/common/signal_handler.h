#pragma once
#include <atomic>
#include <functional>

class SignalHandler {
public:
    static void init(std::function<void()> cb);
    static bool isStopping();

    static std::atomic<bool> stop_;
};