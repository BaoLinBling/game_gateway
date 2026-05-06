#include <gtest/gtest.h>
#include "middleware/brpc/brpc_client.h"
#include <atomic>
#include <thread>

TEST(BrpcTest, LoginSync) {
    BrpcClient client;
    client.setMock(true);

    std::string token = client.LoginSync("user1", "pwd");
    ASSERT_EQ(token, "token_user1");
}

TEST(BrpcTest, LoginAsync) {
    BrpcClient client;
    client.setMock(true);

    std::atomic<bool> done{false};

    client.LoginAsync("user2", "pwd", [&](const std::string& token) {
        ASSERT_EQ(token, "token_user2");
        done = true;
    });

    // 等待回调
    for (int i = 0; i < 50 && !done; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_TRUE(done);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
