#include <gtest/gtest.h>
#include "middleware/kafka/kafka_producer.h"
#include <thread>
#include <vector>

TEST(KafkaTest, SendBasic) {
    KafkaProducer producer;
    producer.send("hello_kafka");
    SUCCEED();
}

TEST(KafkaTest, ConcurrentSend) {
    KafkaProducer producer;

    std::vector<std::thread> threads;

    for (int i = 0; i < 100; ++i) {
        threads.emplace_back([&producer, i]() {
            producer.send("msg_" + std::to_string(i));
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    SUCCEED();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
