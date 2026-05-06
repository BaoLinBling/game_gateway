#pragma once
#include <functional>

namespace coroutine {

class CoroutineScheduler {
public:
    static void init();
    static void run(std::function<void()> fn);
};

}
