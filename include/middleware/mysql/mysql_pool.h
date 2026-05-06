#pragma once
#include "middleware/mysql/mysql_conn.h"
#include <queue>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <map>
#include <condition_variable>

class MySQLPool {
public:
    static MySQLPool& getInstance();
    ~MySQLPool();

    std::shared_ptr<MySQLConn> getConnection(long timeoutMs = 3000);

    bool exec(const std::string& sql);
    std::vector<std::map<std::string, std::string>> query(const std::string& sql);
    bool transaction(std::function<bool(MySQLConn&)> func);

private:
    MySQLPool(int size = 10, int idleTimeoutSec = 30);

    MySQLPool(const MySQLPool&) = delete;
    MySQLPool& operator=(const MySQLPool&) = delete;

    void startIdleCheckThread();
    void checkIdleConnections();

private:
    std::queue<std::shared_ptr<MySQLConn>> pool_;
    std::mutex mtx_;
    std::condition_variable cv_;

    int size_;
    int idle_timeout_sec_;

    std::atomic<bool> running_;
    std::thread check_thread_;
};
