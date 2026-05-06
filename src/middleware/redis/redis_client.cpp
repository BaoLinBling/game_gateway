#include "middleware/redis/redis_client.h"
#include <hiredis/hiredis.h>
#include <thread>
#include <chrono>
#include <iostream>

static const int TTL = 3600;

RedisClient::RedisClient() {
    ctx_ = redisConnect("127.0.0.1", 6379);
    if (!ctx_ || ((redisContext*)ctx_)->err) {
        std::cerr << "redis connect fail" << std::endl;
    }
}

RedisClient::~RedisClient() {
    if (ctx_) {
        redisFree((redisContext*)ctx_);
    }
}

bool RedisClient::setSession(const std::string& user_id, const std::string& data) {
    std::string key = "session:" + user_id;

    redisReply* reply = (redisReply*)redisCommand(
        (redisContext*)ctx_,
        "SETEX %s %d %s",
        key.c_str(), TTL, data.c_str()
    );

    if (!reply) return false;
    bool ok = reply->type == REDIS_REPLY_STATUS;
    freeReplyObject(reply);
    return ok;
}

std::string RedisClient::getSession(const std::string& user_id) {
    std::string key = "session:" + user_id;

    redisReply* reply = (redisReply*)redisCommand(
        (redisContext*)ctx_,
        "GET %s",
        key.c_str()
    );

    std::string res;
    if (reply && reply->type == REDIS_REPLY_STRING) {
        res = reply->str;
    }
    if (reply) freeReplyObject(reply);
    return res;
}

bool RedisClient::tryLock(const std::string& user_id) {
    std::string key = "lock:session:" + user_id;
    redisReply* reply = (redisReply*)redisCommand(
        (redisContext*)ctx_,
        "SET %s 1 NX EX 1",
        key.c_str()
    );
    bool ok = reply && (reply->type == REDIS_REPLY_STATUS);
    if (reply) freeReplyObject(reply);
    return ok;
}

void RedisClient::unlock(const std::string& user_id) {
    std::string key = "lock:session:" + user_id;
    redisReply* reply = (redisReply*)redisCommand((redisContext*)ctx_, "DEL %s", key.c_str());
    if (reply) freeReplyObject(reply);
}
