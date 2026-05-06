#include "network/connection.h"
#include "network/codec.h"

using namespace muduo::net;

namespace network {

Connection::Connection(const TcpConnectionPtr& conn)
    : conn_(conn) {
    buffer_.reserve(1024);
}

void Connection::OnMessage(Buffer* buf) {
    // 直接从 muduo buffer 读取数据，安全
    size_t len = buf->readableBytes();
    if (len > 0) {
        buffer_.insert(buffer_.end(), buf->peek(), buf->peek() + len);
        buf->retrieveAll();
    }

    // 解包循环
    while (true) {
        PacketHeader header;
        std::string payload;

        if (!Codec::Decode(buffer_, header, payload)) break;

        // echo 回包
        auto resp = Codec::Encode(header.cmd, payload);
        conn_->send(resp.data(), resp.size());
    }
}

}
