#pragma once
#include <string>
#include <atomic>

namespace network {

class UdpServer {
public:
    UdpServer(int port = 8081);
    ~UdpServer();

    void run();

private:
    void initSocket();
    void loop();

    void handleMessage(const std::string& msg, const std::string& client);

private:
    int fd_;
    int port_;
    std::atomic<bool> running_;
};

}
