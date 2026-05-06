#pragma once
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <map>
#include <ctime>

class MySQLConn {
public:
    MySQLConn();
    ~MySQLConn();

    bool connect(const std::string& host,
                 int port,
                 const std::string& user,
                 const std::string& pwd,
                 const std::string& db);

    bool exec(const std::string& sql);
    std::vector<std::map<std::string, std::string>> query(const std::string& sql);

    void setLastUsed(time_t t);
    time_t getLastUsed() const;

private:
    MYSQL* conn_;
    time_t last_used_;
};
