#include "network/server.h"
#include "coroutine/coroutine_scheduler.h"

using namespace muduo::net;
using muduo::Timestamp;

namespace network {

Server::Server(EventLoop* loop, int port)
    : server_(loop, InetAddress(port), "GameGateway"),
      thread_pool_(16) {

    server_.setConnectionCallback(
        std::bind(&Server::onConnection, this, std::placeholders::_1));

    server_.setMessageCallback(
        std::bind(&Server::onMessage, this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3));

    server_.setThreadNum(1);
}

void Server::start() {
    server_.start();
}

void Server::onConnection(const TcpConnectionPtr&) {}

void Server::onMessage(const TcpConnectionPtr& conn,
                       Buffer* buf,
                       Timestamp) {

    std::string msg = buf->retrieveAllAsString();

    thread_pool_.submit([this, conn, msg]() {
        coroutine::CoroutineScheduler::init();

        coroutine::CoroutineScheduler::run([this, conn, msg]() {
            std::string resp = login_.handle(msg);

            conn->getLoop()->runInLoop([conn, resp]() {
                if (conn->connected()) {
                    conn->send(resp);
                }
            });
        });
    });
}

}
