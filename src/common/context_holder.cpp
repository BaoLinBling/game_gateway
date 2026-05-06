#include "common/context_holder.h"

thread_local RequestContext* ContextHolder::ctx_ = nullptr;

void ContextHolder::set(RequestContext* ctx) {
    ctx_ = ctx;
}

RequestContext* ContextHolder::get() {
    return ctx_;
}
