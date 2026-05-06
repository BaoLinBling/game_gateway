#pragma once
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include "coroutine/thread_pool.h"
#include "business/login/login_handler.h"

namespace network {

class Server {
public:
    Server(muduo::net::EventLoop* loop, int port);
    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp);

    muduo::net::TcpServer server_;
    coroutine::ThreadPool thread_pool_;
    LoginHandler login_;
};

}
