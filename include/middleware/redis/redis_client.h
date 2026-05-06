#pragma once
#include <string>

class RedisClient {
public:
    RedisClient();
    ~RedisClient();

    bool setSession(const std::string& user_id, const std::string& data);
    std::string getSession(const std::string& user_id);

    bool tryLock(const std::string& user_id);
    void unlock(const std::string& user_id);

private:
    void* ctx_;
};
