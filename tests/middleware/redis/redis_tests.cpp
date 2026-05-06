#include <gtest/gtest.h>
#include "middleware/redis/redis_client.h"

TEST(RedisTest, SessionBasic) {
    RedisClient client;
    client.setSession("1", "abc");
    std::string v = client.getSession("1");
    ASSERT_EQ(v, "abc");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
