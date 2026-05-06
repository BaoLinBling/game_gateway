#include "config/config.h"
#include <yaml-cpp/yaml.h>
#include <cstdlib>
#include <string>

static AppConfig g_config;

static std::string GetEnv(const std::string& key) {
    const char* val = std::getenv(key.c_str());
    return val ? std::string(val) : "";
}

static void OverrideFromEnv() {
    // JWT
    if (!GetEnv("GG_JWT_SECRET").empty())
        g_config.jwt.secret = GetEnv("GG_JWT_SECRET");

    // Network
    if (!GetEnv("GG_NETWORK_PORT").empty())
        g_config.network.port = std::stoi(GetEnv("GG_NETWORK_PORT"));
    if (!GetEnv("GG_NETWORK_BYTE_ORDER").empty())
        g_config.network.byte_order = GetEnv("GG_NETWORK_BYTE_ORDER");

    // MySQL
    if (!GetEnv("GG_MYSQL_HOST").empty())
        g_config.mysql.host = GetEnv("GG_MYSQL_HOST");
    if (!GetEnv("GG_MYSQL_PORT").empty())
        g_config.mysql.port = std::stoi(GetEnv("GG_MYSQL_PORT"));
    if (!GetEnv("GG_MYSQL_USER").empty())
        g_config.mysql.user = GetEnv("GG_MYSQL_USER");
    if (!GetEnv("GG_MYSQL_PASSWORD").empty())
        g_config.mysql.password = GetEnv("GG_MYSQL_PASSWORD");
    if (!GetEnv("GG_MYSQL_DB").empty())
        g_config.mysql.db = GetEnv("GG_MYSQL_DB");
    if (!GetEnv("GG_MYSQL_POOL_SIZE").empty())
        g_config.mysql.pool_size = std::stoi(GetEnv("GG_MYSQL_POOL_SIZE"));

    // Redis
    if (!GetEnv("GG_REDIS_HOST").empty())
        g_config.redis.host = GetEnv("GG_REDIS_HOST");
    if (!GetEnv("GG_REDIS_PORT").empty())
        g_config.redis.port = std::stoi(GetEnv("GG_REDIS_PORT"));
    if (!GetEnv("GG_REDIS_SESSION_TTL").empty())
        g_config.redis.session_ttl = std::stoi(GetEnv("GG_REDIS_SESSION_TTL"));

    // Kafka
    if (!GetEnv("GG_KAFKA_BOOTSTRAP_SERVERS").empty())
        g_config.kafka.bootstrap_servers = GetEnv("GG_KAFKA_BOOTSTRAP_SERVERS");
    if (!GetEnv("GG_KAFKA_TOPIC").empty())
        g_config.kafka.topic = GetEnv("GG_KAFKA_TOPIC");

    // brpc
    if (!GetEnv("GG_BRPC_ENDPOINT").empty())
        g_config.brpc.endpoint = GetEnv("GG_BRPC_ENDPOINT");

    // perf
    if (!GetEnv("GG_PERF_TARGET_QPS").empty())
        g_config.perf.target_qps = std::stoi(GetEnv("GG_PERF_TARGET_QPS"));
}

bool LoadConfig(const std::string& path) {
    try {
        YAML::Node root = YAML::LoadFile(path);

        g_config.jwt.secret = root["jwt"]["secret"].as<std::string>();

        g_config.network.port = root["network"]["port"].as<int>();
        g_config.network.byte_order = root["network"]["byte_order"].as<std::string>();

        g_config.mysql.host = root["mysql"]["host"].as<std::string>();
        g_config.mysql.port = root["mysql"]["port"].as<int>();
        g_config.mysql.user = root["mysql"]["user"].as<std::string>();
        g_config.mysql.password = root["mysql"]["password"].as<std::string>();
        g_config.mysql.db = root["mysql"]["db"].as<std::string>();
        g_config.mysql.pool_size = root["mysql"]["pool_size"].as<int>();

        g_config.redis.host = root["redis"]["host"].as<std::string>();
        g_config.redis.port = root["redis"]["port"].as<int>();
        g_config.redis.session_ttl = root["redis"]["session_ttl"].as<int>();

        g_config.kafka.bootstrap_servers = root["kafka"]["bootstrap_servers"].as<std::string>();
        g_config.kafka.topic = root["kafka"]["topic"].as<std::string>();

        g_config.brpc.endpoint = root["brpc"]["endpoint"].as<std::string>();

        g_config.perf.target_qps = root["perf"]["target_qps"].as<int>();

        OverrideFromEnv();
        return true;
    } catch (...) {
        return false;
    }
}

AppConfig& GetConfig() {
    return g_config;
}
