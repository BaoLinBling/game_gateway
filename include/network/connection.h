#pragma once
#include <muduo/net/TcpConnection.h>
#include <vector>

namespace network {

class Connection {
public:
    explicit Connection(const muduo::net::TcpConnectionPtr& conn);
    void OnMessage(muduo::net::Buffer* buf);

private:
    muduo::net::TcpConnectionPtr conn_;
    std::vector<char> buffer_;
};

}
