#include "common/config.h"
#include <yaml-cpp/yaml.h>

Config& Config::instance() {
    static Config inst;
    return inst;
}

bool Config::load(const std::string& file) {
    YAML::Node root = YAML::LoadFile(file);

    auto g = root["gateway"];
    auto l = root["limiter"];
    auto b = root["breaker"];

    cfg_.zerocopy = g["zerocopy"].as<bool>();
    cfg_.ratelimit = g["ratelimit"].as<bool>();
    cfg_.circuitbreaker = g["circuitbreaker"].as<bool>();

    cfg_.qps = l["qps"].as<int>();
    cfg_.burst = l["burst"].as<int>();

    cfg_.breaker_threshold = b["threshold"].as<double>();
    cfg_.breaker_window_ms = b["window_ms"].as<int>();
    cfg_.breaker_open_ms = b["open_time_ms"].as<int>();

    return true;
}

const GatewayConfig& Config::get() const {
    return cfg_;
}
