#pragma once
#include "common/request_context.h"

class ContextHolder {
public:
    static void set(RequestContext* ctx);
    static RequestContext* get();

private:
    static thread_local RequestContext* ctx_;
};
