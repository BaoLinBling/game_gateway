#include "common/logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <filesystem>

std::shared_ptr<spdlog::logger> Logger::logger_ = nullptr;

void Logger::init(const std::string& log_dir) {
    if (logger_) return;

    std::filesystem::create_directories(log_dir);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_dir + "/gateway.log", 10 * 1024 * 1024, 5);

    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};

    logger_ = std::make_shared<spdlog::logger>("gateway", sinks.begin(), sinks.end());

    logger_->set_level(spdlog::level::info);
    logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    spdlog::register_logger(logger_);
}

std::shared_ptr<spdlog::logger>& Logger::get() {
    return logger_;
}
