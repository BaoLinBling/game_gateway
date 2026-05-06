#include "middleware/kafka/kafka_producer.h"
#include <librdkafka/rdkafka.h>
#include <cstring>

static const char* BROKERS = "127.0.0.1:9092";
static const char* TOPIC = "gateway_logs";

KafkaProducer::KafkaProducer()
    : producer_(nullptr),
      topic_(nullptr),
      available_(false) {

    fallback_file_.open("gateway_fallback.log", std::ios::app);
    init();
}

KafkaProducer::~KafkaProducer() {
    if (producer_) {
        rd_kafka_flush((rd_kafka_t*)producer_, 1000);
        rd_kafka_destroy((rd_kafka_t*)producer_);
    }
    if (fallback_file_.is_open()) {
        fallback_file_.close();
    }
}

void KafkaProducer::init() {
    char errstr[512];

    rd_kafka_conf_t* conf = rd_kafka_conf_new();

    rd_kafka_conf_set(conf, "acks", "1", errstr, sizeof(errstr));
    rd_kafka_conf_set(conf, "bootstrap.servers", BROKERS, errstr, sizeof(errstr));

    rd_kafka_t* rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk) {
        available_ = false;
        return;
    }

    rd_kafka_topic_t* rkt = rd_kafka_topic_new(rk, TOPIC, nullptr);
    if (!rkt) {
        rd_kafka_destroy(rk);
        available_ = false;
        return;
    }

    producer_ = rk;
    topic_ = rkt;
    available_ = true;
}

void KafkaProducer::fallback(const std::string& msg) {
    if (fallback_file_.is_open()) {
        fallback_file_ << msg << std::endl;
    }
}

void KafkaProducer::send(const std::string& msg) {
    if (!available_) {
        fallback(msg);
        return;
    }

    int ret = rd_kafka_produce(
        (rd_kafka_topic_t*)topic_,
        RD_KAFKA_PARTITION_UA,
        RD_KAFKA_MSG_F_COPY,
        (void*)msg.data(),
        msg.size(),
        nullptr,
        0,
        nullptr
    );

    if (ret == -1) {
        fallback(msg);
    } else {
        rd_kafka_poll((rd_kafka_t*)producer_, 0);
    }
}
