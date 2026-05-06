#pragma once
#include <hiredis/hiredis.h>
#include <string>
#include <co_routine.h>

class RedisClient {
public:
    static RedisClient& instance() {
        static RedisClient cli;
        return cli;
    }

    bool connect(const char* ip = "127.0.0.1", int port = 6379) {
        ctx_ = redisConnect(ip, port);
        return ctx_ && !ctx_->err;
    }

    void set(const std::string& k, const std::string& v) {
        redisCommand(ctx_, "SET %s %s", k.c_str(), v.c_str());
    }

    std::string get(const std::string& k) {
        redisReply* r = (redisReply*)redisCommand(ctx_, "GET %s", k.c_str());
        if (!r || r->type != REDIS_REPLY_STRING) {
            if (r) freeReplyObject(r);
            return "";
        }
        std::string v = r->str;
        freeReplyObject(r);
        return v;
    }

private:
    redisContext* ctx_ = nullptr;
};
