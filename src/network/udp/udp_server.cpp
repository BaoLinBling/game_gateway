#include "network/udp/udp_server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>
#include <hiredis/hiredis.h>

namespace network {

static const char* REDIS_HOST = "127.0.0.1";
static const int REDIS_PORT = 6379;
static const char* CHANNEL = "udp_broadcast";

UdpServer::UdpServer(int port)
    : fd_(-1), port_(port), running_(true) {}

UdpServer::~UdpServer() {
    running_ = false;
    if (fd_ != -1) close(fd_);
}

void UdpServer::initSocket() {
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);

    int flags = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(fd_, (sockaddr*)&addr, sizeof(addr));
}

void UdpServer::handleMessage(const std::string& msg, const std::string&) {
    redisContext* ctx = redisConnect(REDIS_HOST, REDIS_PORT);
    if (!ctx || ctx->err) return;

    redisCommand(ctx, "PUBLISH %s %s", CHANNEL, msg.c_str());
    redisFree(ctx);
}

void UdpServer::loop() {
    int epfd = epoll_create1(0);

    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd_;

    epoll_ctl(epfd, EPOLL_CTL_ADD, fd_, &ev);

    epoll_event events[1024];

    while (running_) {
        int n = epoll_wait(epfd, events, 1024, 10);

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == fd_) {
                while (true) {
                    char buf[1024];
                    sockaddr_in client{};
                    socklen_t len = sizeof(client);

                    int ret = recvfrom(fd_, buf, sizeof(buf), 0,
                                        (sockaddr*)&client, &len);

                    if (ret <= 0) break;

                    std::string msg(buf, ret);
                    char ip[64];
                    inet_ntop(AF_INET, &client.sin_addr, ip, sizeof(ip));

                    handleMessage(msg, ip);
                }
            }
        }
    }

    close(epfd);
}

void UdpServer::run() {
    initSocket();
    loop();
}

}
