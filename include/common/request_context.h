#pragma once
#include <string>
#include <chrono>

struct RequestContext {
    std::string trace_id;
    int64_t start_time_ms;
};
