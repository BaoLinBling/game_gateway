#pragma once
#include <string>
#include <memory>
#include "middleware/mysql/mysql_pool.h"
#include "middleware/redis/redis_client.h"
#include "middleware/kafka/kafka_producer.h"

class LoginHandler {
public:
    LoginHandler();

    std::string handle(const std::string& req);

private:
    std::string queryUserFromDb(const std::string& user);
    std::string getSessionFromCache(const std::string& user);
    void setSession(const std::string& user, const std::string& token);
    std::string generateJwt(const std::string& user);

private:
    MySQLPool& mysql_;
    RedisClient redis_;
    KafkaProducer kafka_;
};
