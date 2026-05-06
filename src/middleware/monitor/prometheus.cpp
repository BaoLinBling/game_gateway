/*#include "middleware/monitor/prometheus.h"
#include <sstream>

Prometheus::Prometheus()
    : requests_total_(0),
      conn_count_(0),
      coroutine_count_(0),
      memory_pool_used_(0),
      latency_sum_(0),
      latency_count_(0) {}

Prometheus& Prometheus::getInstance() {
    static Prometheus inst;
    return inst;
}

void Prometheus::incRequests() {
    requests_total_++;
}

void Prometheus::incConn() {
    conn_count_++;
}

void Prometheus::decConn() {
    conn_count_--;
}

void Prometheus::incCoroutine() {
    coroutine_count_++;
}

void Prometheus::decCoroutine() {
    coroutine_count_--;
}

void Prometheus::setMemoryUsed(size_t v) {
    memory_pool_used_ = v;
}

void Prometheus::observeLatency(double ms) {
    latency_sum_ += ms;
    latency_count_++;
}

//轻量级
void Prometheus::incConnections() {
    connections_++;
}

void Prometheus::decConnections() {
    connections_--;
}

void Prometheus::observeLatency(double ms) {
    total_latency_ += (long long)ms;
}

/*std::string Prometheus::metrics() {
    std::ostringstream ss;

    double avg = 0;
    long cnt = latency_count_.load();
    if (cnt > 0) {
        avg = latency_sum_.load() / cnt;
    }

    ss <<
        "requests_total " << requests_total_.load() << "\n"
        "conn_count " << conn_count_.load() << "\n"
        "coroutine_count " << coroutine_count_.load() << "\n"
        "memory_pool_used " << memory_pool_used_.load() << "\n"
        "latency_avg " << avg << "\n";

    return ss.str();
}
*/
/*std::string Prometheus::metrics() {
    std::ostringstream oss;

    oss << "gateway_requests_total " << requests_.load() << "\n";
    oss << "gateway_connections " << connections_.load() << "\n";
    oss << "gateway_latency_ms_total " << total_latency_.load() << "\n";

    return oss.str();
}*/
#include "middleware/monitor/prometheus.h"
#include <sstream>

Prometheus::Prometheus()
    : requests_total_(0),
      conn_count_(0),
      coroutine_count_(0),
      memory_pool_used_(0),
      latency_sum_(0),
      latency_count_(0)
{
}

Prometheus& Prometheus::getInstance() {
    static Prometheus inst;
    return inst;
}

void Prometheus::incRequests() {
    requests_total_++;
}

void Prometheus::incConn() {
    conn_count_++;
}

void Prometheus::decConn() {
    conn_count_--;
}

void Prometheus::incCoroutine() {
    coroutine_count_++;
}

void Prometheus::decCoroutine() {
    coroutine_count_--;
}

void Prometheus::setMemoryUsed(size_t v) {
    memory_pool_used_ = v;
}

// 轻量级统计
void Prometheus::observeLatency(double ms) {
    total_latency_ += (long long)ms;
}

void Prometheus::incConnections() {
    connections_++;
}

void Prometheus::decConnections() {
    connections_--;
}

std::string Prometheus::metrics() {
    std::ostringstream oss;
    oss << "gateway_requests_total " << requests_total_.load() << "\n";
    oss << "gateway_connections " << connections_.load() << "\n";
    oss << "gateway_latency_ms_total " << total_latency_.load() << "\n";

    return oss.str();
}