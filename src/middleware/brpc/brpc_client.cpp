#include "middleware/brpc/brpc_client.h"
#include <thread>
#include <chrono>

BrpcClient::BrpcClient() : mock_(true) {}

void BrpcClient::setMock(bool enable) {
    mock_ = enable;
}

// 内部登录实现（包括重试）
std::string BrpcClient::doLogin(const std::string& user, const std::string& pwd) {
    int retries = 3;

    for (int i = 0; i < retries; ++i) {
        // Mock 模式
        if (mock_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return "token_" + user;
        }

        // 模拟 RPC 超时控制（500ms）
        auto start = std::chrono::steady_clock::now();

        // 模拟网络请求（这里用 sleep 模拟）
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto end = std::chrono::steady_clock::now();
        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (cost <= 500) {
            return "token_" + user;
        }
    }

    return "";
}

// 同步接口
std::string BrpcClient::LoginSync(const std::string& user, const std::string& pwd) {
    return doLogin(user, pwd);
}

// 异步接口
void BrpcClient::LoginAsync(const std::string& user,
                            const std::string& pwd,
                            std::function<void(const std::string&)> cb) {

    std::thread([=]() {
        std::string res = doLogin(user, pwd);
        if (cb) {
            cb(res);
        }
    }).detach();
}
