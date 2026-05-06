#include "business/login/login_handler.h"
#include <sstream>
#include <chrono>
#include <iostream>

LoginHandler::LoginHandler()
    : mysql_(MySQLPool::getInstance()) {}

std::string LoginHandler::handle(const std::string& req) {
    std::string user = req;

    // 1. Redis session
    std::string session = getSessionFromCache(user);
    if (!session.empty()) {
        return session;
    }

    // 2. MySQL
    std::string dbData = queryUserFromDb(user);

    // 3. JWT
    std::string token = generateJwt(user);

    // 4. Redis cache
    setSession(user, token);

    // 5. Kafka log
    kafka_.send("login success: " + user);

    return token;
}

std::string LoginHandler::queryUserFromDb(const std::string& user) {
    std::string sql = "SELECT name FROM user WHERE name='" + user + "';";
    auto res = mysql_.query(sql);

    if (!res.empty()) {
        return res[0]["name"];
    }
    return "";
}

std::string LoginHandler::getSessionFromCache(const std::string& user) {
    return redis_.getSession(user);
}

void LoginHandler::setSession(const std::string& user, const std::string& token) {
    redis_.setSession(user, token);
}

std::string LoginHandler::generateJwt(const std::string& user) {
    std::stringstream ss;
    ss << "jwt_" << user << "_" << time(nullptr);
    return ss.str();
}
