#pragma once
#include <string>
#include <atomic>
#include <mutex>

class Prometheus {
public:
    static Prometheus& getInstance();

    void incRequests();
    void incConn();
    void decConn();
    void incCoroutine();
    void decCoroutine();

    void setMemoryUsed(size_t v);
    //void observeLatency(double ms);

    //轻量化生产及能力
    void incConnections();
    void decConnections();
    void observeLatency(double ms);

    std::string metrics();

private:
    Prometheus();
    std::atomic<int> connections_{0};
    std::atomic<long long> total_latency_{0};

    std::atomic<long> requests_total_;
    std::atomic<long> conn_count_;
    std::atomic<long> coroutine_count_;

    std::atomic<size_t> memory_pool_used_;

    std::atomic<double> latency_sum_;
    std::atomic<long> latency_count_;

    std::mutex mtx_;
};
