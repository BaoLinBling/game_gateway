#include <gtest/gtest.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <string>
#include <cstring>

#include "network/codec.h"

using namespace network;

static void set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static ssize_t write_all(int fd, const std::string& data) {
    return write(fd, data.data(), data.size());
}

static void et_read_loop(int fd, std::vector<char>& buffer) {
    char tmp[1024];
    while (true) {
        ssize_t n = read(fd, tmp, sizeof(tmp));
        if (n > 0) {
            buffer.insert(buffer.end(), tmp, tmp + n);
        } else if (n == -1 && errno == EAGAIN) {
            break;
        } else {
            break;
        }
    }
}

TEST(NetworkTest, PartialPacket) {
    std::vector<char> buffer;
    std::string payload = "hello";
    auto pkt = Codec::Encode(1001, payload);

    std::vector<char> part1(pkt.begin(), pkt.begin() + 4);
    std::vector<char> part2(pkt.begin() + 4, pkt.end());

    buffer.insert(buffer.end(), part1.begin(), part1.end());
    PacketHeader header;
    std::string out;
    EXPECT_FALSE(Codec::Decode(buffer, header, out));

    buffer.insert(buffer.end(), part2.begin(), part2.end());
    EXPECT_TRUE(Codec::Decode(buffer, header, out));
    EXPECT_EQ(out, payload);
    EXPECT_EQ(header.cmd, 1001);
}

TEST(NetworkTest, StickyPacket) {
    std::vector<char> buffer;
    auto pkt1 = Codec::Encode(1001, "hello");
    auto pkt2 = Codec::Encode(1002, "world");

    buffer.insert(buffer.end(), pkt1.begin(), pkt1.end());
    buffer.insert(buffer.end(), pkt2.begin(), pkt2.end());

    PacketHeader header;
    std::string out;
    EXPECT_TRUE(Codec::Decode(buffer, header, out));
    EXPECT_EQ(out, "hello");
    EXPECT_EQ(header.cmd, 1001);

    EXPECT_TRUE(Codec::Decode(buffer, header, out));
    EXPECT_EQ(out, "world");
    EXPECT_EQ(header.cmd, 1002);
}

TEST(NetworkTest, ETReadUntilEAGAIN) {
    int fds[2];
    ASSERT_EQ(pipe(fds), 0);
    int read_fd = fds[0];
    int write_fd = fds[1];

    set_nonblock(read_fd);
    std::string pkt = Codec::Encode(1001, "et_test");
    write_all(write_fd, pkt);

    std::vector<char> buffer;
    et_read_loop(read_fd, buffer);
    EXPECT_EQ(buffer.size(), pkt.size());

    char tmp[16];
    ssize_t n = read(read_fd, tmp, sizeof(tmp));
    EXPECT_EQ(n, -1);
    EXPECT_EQ(errno, EAGAIN);

    close(read_fd);
    close(write_fd);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
