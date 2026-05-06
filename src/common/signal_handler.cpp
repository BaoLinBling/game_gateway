#include "common/signal_handler.h"
#include <csignal>

std::atomic<bool> SignalHandler::stop_ = false;

static std::function<void()> g_callback;

void handleSignal(int) {
    SignalHandler::stop_ = true;

    if (g_callback) {
        g_callback();  // 通知主线程
    }
}

void SignalHandler::init(std::function<void()> cb) {
    g_callback = cb;

    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
}

bool SignalHandler::isStopping() {
    return stop_.load();
}