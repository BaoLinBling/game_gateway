#pragma once
#include <string>

struct GatewayConfig {
    bool zerocopy = true;
    bool ratelimit = true;
    bool circuitbreaker = true;

    int qps = 5000;
    int burst = 10000;

    double breaker_threshold = 0.05;
    int breaker_window_ms = 10000;
    int breaker_open_ms = 5000;
};

class Config {
public:
    static Config& instance();

    bool load(const std::string& file);

    const GatewayConfig& get() const;

private:
    GatewayConfig cfg_;
};
