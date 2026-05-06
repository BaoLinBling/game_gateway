#include "common/config_manager.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <mutex>

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

void ConfigManager::load(const std::string& file) {
    file_ = file;
    try {
        YAML::Node config = YAML::LoadFile(file);

        std::unique_lock<std::shared_mutex> lock(mutex_);
        int_configs_.clear();

        for (auto it : config) {
            // 只加载顶层标量类型（数字/字符串可转数字）
            // 自动跳过jwt/mysql/redis这种嵌套节点
            if (it.second.IsScalar()) {
                try {
                    int val = it.second.as<int>();
                    std::string key = it.first.as<std::string>();
                    int_configs_[key] = val;
                } catch (...) {
                    // 字符串跳过，不报错
                }
            }
        }
    } catch (...) {
        // 这样做加载失败不会崩溃
    }
}

int ConfigManager::getInt(const std::string& key, int default_val) {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = int_configs_.find(key);
    if (it != int_configs_.end()) {
        return it->second;
    }
    return default_val;
}

void ConfigManager::startAutoReload(int interval_sec) {
    if (running_) return;
    running_ = true;

    std::thread([this, interval_sec]() {
        reloadLoop(interval_sec);
    }).detach();
}

void ConfigManager::reloadLoop(int interval_sec) {
    while (running_) {
        try {
            load(file_);
        } catch (...) {
        }
        std::this_thread::sleep_for(std::chrono::seconds(interval_sec));
    }
}
