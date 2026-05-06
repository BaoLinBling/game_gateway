# Game Gateway（实时高并发游戏网关）

一款基于 C++17、C++20、协程、muduo 网络库开发的高并发、高性能实时游戏网关。

## 技术栈
- C++17
- muduo 网络库
- libco 协程
- MySQL + Redis
- Kafka 消息队列
- Protobuf + gRPC
- YAML 配置

## 环境要求
- Ubuntu 22.04
- GCC >= 11
- CMake >= 3.10

## 快速开始
# 编译
./build.sh

# 启动
./start.sh


#########################################################################


# 标准化稳定运行命令

# 1. 清理残留进程
sudo lsof -t -i:8080 -i:9090 | xargs kill -9

# 2. 全项目重新编译
cd build && cmake .. && make -j$(nproc) && cd ..

# 3. 启动准游戏网关
./build/game_gateway

# 4. 验证监控
curl http://127.0.0.1:9090/metrics

# 5. 验证网关
telnet 127.0.0.1 8080

# 6. C++20 协程性能压测
bash scripts/bench_cpp20.sh