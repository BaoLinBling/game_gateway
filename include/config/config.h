#pragma once
#include <string>

class Config {
public:
    static Config& Get() {
        static Config inst;
        return inst;
    }

    // 完全匹配你的 dev.yaml Docker MySQL
    std::string mysql_host = "127.0.0.1";
    int mysql_port = 13306;
    std::string mysql_user = "root";
    std::string mysql_pwd = "secret";
    std::string mysql_db = "game_gateway";

private:
    Config() = default;
};
