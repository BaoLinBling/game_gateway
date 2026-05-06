#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/InetAddress.h>
#include "middleware/monitor/prometheus.h"
#include <iostream>
#include <string>
#include "common/logger.h"

#include "middleware/limiter/token_bucket.h"
#include "middleware/breaker/circuit_breaker.h"
#include <chrono>

#include "common/signal_handler.h"

#include "common/config_manager.h"
#include "common/context_holder.h"
#include "common/request_context.h"
#include "common/trace.h"

using namespace muduo::net;
using namespace muduo;

// 网关消息处理（echo模式）
/*void onGatewayMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp) {
    //轻量级
    static TokenBucket limiter(1000, 1000);
    static CircuitBreaker breaker(0.5, 10000, 5000);

    auto start = std::chrono::steady_clock::now();

    if (!limiter.allow()) {
        Logger::get()->warn("Rate limited");
        return;
    }

    if (!breaker.allow()) {
        Logger::get()->error("Circuit breaker open");
        return;
    }

    conn->send(buf->retrieveAllAsString());

    auto end = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    Prometheus::getInstance().incRequests();
    Prometheus::getInstance().observeLatency(ms);
}
*/
void onGatewayMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp) {
    RequestContext ctx;
    ctx.trace_id = Trace::generateTraceId();
    ctx.start_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();

    ContextHolder::set(&ctx);

    Logger::get()->info("trace_id={} new request", ctx.trace_id);

    int rate = ConfigManager::instance().getInt("rate_limit", 1000);
    static TokenBucket limiter(rate, rate);

    //static CircuitBreaker breaker;
    // 从配置读取，没有则用默认值
    int threshold = ConfigManager::instance().getInt("breaker_threshold", 5);    // 5%错误率
    int window_ms = ConfigManager::instance().getInt("breaker_window", 10000); // 10s窗口
    int open_ms = ConfigManager::instance().getInt("breaker_open", 5000);       // 熔断5s
    static CircuitBreaker breaker(threshold * 0.01, window_ms, open_ms);

    if (!limiter.allow()) {
        Logger::get()->warn("trace_id={} rate limited", ctx.trace_id);
        return;
    }

    if (!breaker.allow()) {
        Logger::get()->error("trace_id={} breaker open", ctx.trace_id);
        return;
    }

    auto start = std::chrono::steady_clock::now();

    conn->send(buf->retrieveAllAsString());

    auto end = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    Prometheus::getInstance().incRequests();
    Prometheus::getInstance().observeLatency(ms);

    Logger::get()->info("trace_id={} done cost={}ms", ctx.trace_id, ms);
}

// 监控接口处理
void onMetricsRequest(const TcpConnectionPtr& conn, Buffer* buf, Timestamp) {
    Prometheus::getInstance().incRequests();
    std::string metrics_data = Prometheus::getInstance().metrics();

    std::string http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(metrics_data.size()) + "\r\n"
        "\r\n" + metrics_data;

    conn->send(http_response);
}

int main() {

    //ConfigManager::instance().load("config/dev.yaml");
    ConfigManager::instance().load("/home/blingbling/projects/game_gateway/config/dev.yaml");
    ConfigManager::instance().startAutoReload(5);
    Logger::init();
    Logger::get()->info("Logger initialized");

    // 须先定义 loop
    EventLoop loop;

    // 捕获&loop，且正确嵌套
    SignalHandler::init([&loop]() {
        Logger::get()->info("Signal received, quitting loop...");
        loop.queueInLoop([&loop]() {
            loop.quit();
        });
    });

    // 网关 8080
    TcpServer gateway_server(&loop, InetAddress(8080), "game_gateway");
    gateway_server.setMessageCallback(onGatewayMessage);
    gateway_server.start();

    // 监控 9090
    TcpServer metrics_server(&loop, InetAddress(9090), "metrics");
    metrics_server.setMessageCallback(onMetricsRequest);
    metrics_server.start();

    Logger::get()->info("GameGateway 8080 启动成功");
    Logger::get()->info("Metrics 9090 启动成功");

    // 正确运行
    loop.loop();

    Logger::get()->info("Server shutting down...");

    return 0;
}