#pragma once
#include <memory>
#include <spdlog/spdlog.h>

class Logger {
public:
    static void init(const std::string& log_dir = "logs");
    static std::shared_ptr<spdlog::logger>& get();

private:
    static std::shared_ptr<spdlog::logger> logger_;
};
