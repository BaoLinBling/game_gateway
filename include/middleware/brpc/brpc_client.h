#pragma once
#include <string>
#include <functional>

class BrpcClient {
public:
    BrpcClient();

    // 同步登录
    std::string LoginSync(const std::string& user, const std::string& pwd);

    // 异步登录
    void LoginAsync(const std::string& user,
                    const std::string& pwd,
                    std::function<void(const std::string&)> cb);

    // Mock 开关
    void setMock(bool enable);

private:
    std::string doLogin(const std::string& user, const std::string& pwd);

private:
    bool mock_;
};
