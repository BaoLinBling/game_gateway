#include "middleware/mysql/mysql_pool.h"
#include <chrono>

static const std::string HOST = "127.0.0.1";
static const int PORT = 13306;
static const std::string USER = "root";
static const std::string PWD = "secret";
static const std::string DB = "game_gateway";

MySQLPool::MySQLPool(int size, int idleTimeoutSec)
    : size_(size),
      idle_timeout_sec_(idleTimeoutSec),
      running_(true) {

    for (int i = 0; i < size_; ++i) {
        auto conn = std::make_shared<MySQLConn>();
        conn->connect(HOST, PORT, USER, PWD, DB);
        conn->setLastUsed(time(nullptr));
        pool_.push(conn);
    }

    startIdleCheckThread();
}

MySQLPool& MySQLPool::getInstance() {
    static MySQLPool inst(10, 30);
    return inst;
}

MySQLPool::~MySQLPool() {
    running_ = false;
    if (check_thread_.joinable()) check_thread_.join();
}

void MySQLPool::startIdleCheckThread() {
    check_thread_ = std::thread(&MySQLPool::checkIdleConnections, this);
}

void MySQLPool::checkIdleConnections() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::lock_guard<std::mutex> lock(mtx_);

        int n = pool_.size();
        for (int i = 0; i < n; ++i) {
            auto conn = pool_.front();
            pool_.pop();

            if (time(nullptr) - conn->getLastUsed() > idle_timeout_sec_) {
                auto new_conn = std::make_shared<MySQLConn>();
                if (new_conn->connect(HOST, PORT, USER, PWD, DB)) {
                    new_conn->setLastUsed(time(nullptr));
                    pool_.push(new_conn);
                } else {
                    pool_.push(conn);
                }
            } else {
                pool_.push(conn);
            }
        }
    }
}

std::shared_ptr<MySQLConn> MySQLPool::getConnection(long timeoutMs) {
    std::unique_lock<std::mutex> lock(mtx_);

    if (!cv_.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                      [this]() { return !pool_.empty(); })) {
        return nullptr;
    }

    auto conn = pool_.front();
    pool_.pop();
    conn->setLastUsed(time(nullptr));
    return conn;
}

bool MySQLPool::exec(const std::string& sql) {
    auto conn = getConnection();
    if (!conn) return false;

    bool ok = conn->exec(sql);

    {
        std::lock_guard<std::mutex> lock(mtx_);
        pool_.push(conn);
    }
    cv_.notify_one();
    return ok;
}

std::vector<std::map<std::string, std::string>> MySQLPool::query(const std::string& sql) {
    auto conn = getConnection();
    if (!conn) return {};

    auto res = conn->query(sql);

    {
        std::lock_guard<std::mutex> lock(mtx_);
        pool_.push(conn);
    }
    cv_.notify_one();
    return res;
}

bool MySQLPool::transaction(std::function<bool(MySQLConn&)> func) {
    auto conn = getConnection();
    if (!conn) return false;

    conn->exec("BEGIN");
    bool ok = func(*conn);
    ok ? conn->exec("COMMIT") : conn->exec("ROLLBACK");

    {
        std::lock_guard<std::mutex> lock(mtx_);
        pool_.push(conn);
    }
    cv_.notify_one();
    return ok;
}
