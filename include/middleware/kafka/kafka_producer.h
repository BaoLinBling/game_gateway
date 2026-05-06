#pragma once
#include <string>
#include <atomic>
#include <fstream>

class KafkaProducer {
public:
    KafkaProducer();
    ~KafkaProducer();

    void send(const std::string& msg);

private:
    void init();
    void fallback(const std::string& msg);

private:
    void* producer_;   // rd_kafka_t*
    void* topic_;      // rd_kafka_topic_t*
    std::atomic<bool> available_;
    std::ofstream fallback_file_;
};
