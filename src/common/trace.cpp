#include "common/trace.h"
#include <random>
#include <sstream>

std::string Trace::generateTraceId() {
    static thread_local std::mt19937_64 rng(std::random_device{}());
    std::stringstream ss;
    ss << std::hex << rng();
    return ss.str();
}
