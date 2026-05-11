
cmake_minimum_required(VERSION 3.10)
project(game_gateway)

//兼容 17 ，支持 20
set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fcoroutines")

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(spdlog REQUIRED)

include_directories(include)
include_directories(third_party/libco)
link_directories(third_party/libco/build)

# --------------------------
# 公共配置模块
# --------------------------
add_library(common_config src/common/config.cpp)
target_link_libraries(common_config yaml-cpp)
add_library(logger src/common/logger.cpp)
target_link_libraries(logger spdlog::spdlog)
target_include_directories(logger PUBLIC include)

# --------------------------
# 限流模块
# --------------------------
add_library(limiter src/middleware/limiter/token_bucket.cpp)
target_include_directories(limiter PUBLIC include)

# --------------------------
# 熔断模块
# --------------------------
add_library(breaker src/middleware/breaker/circuit_breaker.cpp)
target_include_directories(breaker PUBLIC include)

# --------------------------
# UDP 服务模块
# --------------------------
add_library(udp src/network/udp/udp_server.cpp)
target_include_directories(udp PUBLIC include)
target_link_libraries(udp hiredis pthread)

# --------------------------
# 网络模块
# --------------------------
add_library(network
    src/network/server.cpp
    src/network/codec.cpp
    src/network/connection.cpp
    src/network/udp/udp_server.cpp
)
target_include_directories(network PUBLIC include)
target_link_libraries(network
    pthread
    muduo_net
    muduo_base
    redis_client
    hiredis
    udp
    common_config
    limiter
    breaker
)

# --------------------------
# 协程模块
# --------------------------

include_directories(include)

add_library(coroutine
    src/coroutine/thread_pool.cpp
    src/coroutine/coroutine_scheduler.cpp
    src/coroutine_cpp20/advanced_scheduler.cpp
)
target_include_directories(coroutine PUBLIC include)
target_link_libraries(coroutine co pthread)

# --------------------------
# 内存池模块
# --------------------------
add_library(memory src/memory/memory_pool.cpp)
target_include_directories(memory PUBLIC include)
target_link_libraries(memory pthread)

# --------------------------
# 中间件
# --------------------------
add_library(mysql_pool src/middleware/mysql/mysql_pool.cpp src/middleware/mysql/mysql_conn.cpp)
target_link_libraries(mysql_pool mariadb)

add_library(monitor src/middleware/monitor/prometheus.cpp)
target_include_directories(monitor PUBLIC include)

add_library(redis_client src/middleware/redis/redis_client.cpp)
target_link_libraries(redis_client hiredis)

add_library(kafka_producer src/middleware/kafka/kafka_producer.cpp)
target_link_libraries(kafka_producer rdkafka)

add_library(brpc_client src/middleware/brpc/brpc_client.cpp)
target_link_libraries(brpc_client protobuf)

# --------------------------
# 登录业务
# --------------------------
add_library(business_login src/business/login/login_handler.cpp)
target_include_directories(business_login PUBLIC include)
target_link_libraries(business_login
    network
    coroutine
    memory
    mysql_pool
    redis_client
    kafka_producer
    brpc_client
    common_config
    limiter
    breaker
)

# --------------------------
# 主程序，最基础库
# --------------------------
add_executable(game_gateway src/main.cpp)
target_link_libraries(game_gateway
    muduo_net
    muduo_base
    monitor
    logger
    signal_handler
    limiter      
    breaker

    yaml-cpp
    config_manager
    context_holder
    trace

    pthread
)

# --------------------------
# 测试
# --------------------------
enable_testing()
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)

add_library(signal_handler src/common/signal_handler.cpp)
target_include_directories(signal_handler PUBLIC include)

add_library(config_manager src/common/config_manager.cpp)
add_library(context_holder src/common/context_holder.cpp)
add_library(trace src/common/trace.cpp)

target_link_libraries(config_manager yaml-cpp)
target_include_directories(config_manager PUBLIC include)
target_include_directories(context_holder PUBLIC include)
target_include_directories(trace PUBLIC include)

add_executable(full_benchmark tests/benchmark/full_benchmark.cpp)
target_link_libraries(full_benchmark coroutine pthread)
