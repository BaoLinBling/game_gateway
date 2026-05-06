#pragma once
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <thread>
#include <atomic>

class ConfigManager {
public:
    static ConfigManager& instance();

    void load(const std::string& file);
    void startAutoReload(int interval_sec = 5);

    int getInt(const std::string& key, int default_val);

private:
    void reloadLoop(int interval_sec);

private:
    std::unordered_map<std::string, int> int_configs_;
    std::shared_mutex mutex_;
    std::string file_;
    std::atomic<bool> running_{false};
};
