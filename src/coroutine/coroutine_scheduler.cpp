#include "coroutine/coroutine_scheduler.h"
#include <co_routine.h>

namespace coroutine {

void CoroutineScheduler::init() {
    co_enable_hook_sys();
}

static void* coEntry(void* arg) {
    auto fn = static_cast<std::function<void()>*>(arg);
    (*fn)();
    delete fn;
    return nullptr;
}

void CoroutineScheduler::run(std::function<void()> fn) {
    stCoRoutine_t* co = nullptr;
    auto* task = new std::function<void()>(std::move(fn));

    co_create(&co, nullptr, coEntry, task);
    co_resume(co);
}

}
