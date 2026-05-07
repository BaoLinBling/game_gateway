# game_gateway
高并发高性能游戏网关服务

## 项目简介
game_gateway 是基于 C++17 为主、C++20 为辅，结合 Muduo 网络库开发的生产级游戏网关服务。底层基于 Linux epoll 实现 IO 多路复用与 Reactor 事件驱动模型，作为游戏服务端架构核心接入层，负责海量客户端长连接管理、协议解析、消息路由转发与中间件交互。
项目完成网络架构设计、协程调度升级、性能深度调优、工程模块化重构、高可用可观测性建设与容器化部署，兼具高并发承载、低延迟吞吐、服务稳定可靠与生产级可运维特性，适用于游戏长连接网关、海量长连接接入等高并发业务场景。
## 技术栈
- 开发语言：C++17（项目主版本），C++20（无栈协程实验调度模块）
- 网络框架：Muduo Reactor 反应堆模型（底层基于 epoll 多路复用）
- 并发模型：自研多线程协程调度器、C++20 标准无栈协程
- 构建工具：CMake、Shell 脚本
- 通信协议：Protobuf、TCP、UDP
- 中间件：MySQL、Redis、Kafka、ZooKeeper
- 监控体系：Prometheus、Grafana
- 日志组件：spdlog
- 部署方案：Docker、Docker Compose
- 测试工具：Google Test、Benchmark 基准压测、性能采样分析

## 快速开始
### 环境依赖
- Ubuntu 22.04 / WSL2
- GCC 10 及以上版本
- CMake 3.20 及以上版本
- Muduo 网络库
- Protobuf 开发库
- Docker 与 Docker Compose（可选，用于中间件编排）

### 编译构建
# 1. 在任意目录下执行：克隆项目
git clone https://github.com/BaoLinBling/game_gateway.git
cd game_gateway

# 2. 在项目根目录下执行：赋予脚本执行权限
chmod +x build.sh

# 3. 在项目根目录下执行：一键编译所有模块
./build.sh

# 手动编译方式（备选）
# 在项目根目录执行
mkdir -p build && cd build
cmake ..
make -j$(nproc)

# build 目录下单独编译指定模块
# 编译主网关服务
make game_gateway
# 编译 C++20 协程基准测试
make coroutine_bench
# 编译单元测试模块
make unit_tests

# 手动编译 Protobuf 协议文件（项目根目录执行）
protoc --cpp_out=./src/proto/ proto/user_service.proto
protoc --cpp_out=./src/proto/ proto/udp.proto

### 服务启动
# 方式一：项目根目录执行，脚本启动主服务
chmod +x start.sh
./start.sh

# 方式二：项目根目录执行，直接运行编译二进制
./bin/game_gateway

# 方式三：项目根目录执行，启动全套关联服务
chmod +x scripts/start_all.sh
./scripts/start_all.sh

# 方式四：项目根目录执行，停止所有服务
chmod +x scripts/stop_all.sh
./scripts/stop_all.sh

# 方式五：项目根目录执行，容器化编排中间件
docker-compose up -d

### 测试与性能基准
# 运行 C++20 协程基准压测
chmod +x scripts/bench_cpp20.sh
./scripts/bench_cpp20.sh

# 运行系统压力测试
chmod +x scripts/stress_test.sh
./scripts/stress_test.sh

# 运行性能采样分析
chmod +x scripts/perf_test.sh
./scripts/perf_test.sh

# 运行单元测试
./bin/unit_tests

## 项目结构
game_gateway/
├── CMakeLists.txt # 项目主构建配置
├── Dockerfile # 网关容器构建文件
├── build.sh # 一键编译脚本
├── start.sh # 主服务启动脚本
├── bin/ # 编译输出可执行文件
├── build/ # CMake 临时编译目录
├── cmake/ # CMake 模块化配置
├── config/ # 项目配置文件
├── db/ # 数据库初始化脚本
├── deploy/ # 容器化及部署配置
├── docs/ # 项目相关文档
├── logs/ # 服务运行日志目录
├── proto/ # Protobuf 协议定义文件
├── scripts/ # 运维、压测、性能脚本
├── include/ # 项目头文件根目录
│ ├── business/ # 业务逻辑模块
│ ├── common/ # 公共工具基础模块
│ ├── config/ # 配置解析模块
│ ├── coroutine/ # 自研协程基础模块
│ ├── coroutine_cpp20/ # C++20 协程实验模块
│ ├── memory/ # 内存管理模块
│ ├── middleware/ # 第三方中间件封装
│ └── network/ # 网络底层通信模块
├── src/ # 项目源码根目录
│ ├── main.cpp # 程序入口文件
│ ├── metrics_server.cpp # 监控指标服务
│ ├── business/ # 业务逻辑实现
│ ├── common/ # 公共工具实现
│ ├── config/ # 配置解析实现
│ ├── coroutine/ # 自研协程实现
│ ├── coroutine_cpp20/ # C++20 协程实现
│ ├── memory/ # 内存管理实现
│ ├── middleware/ # 中间件逻辑实现
│ ├── network/ # 网络通信实现
│ └── proto/ # Protobuf 生成代码
├── tests/ # 单元测试与业务测试代码
└── third_party/ # 第三方依赖库

## 核心特性
- 基于 Muduo Reactor + Linux epoll 事件驱动模型，支撑海量长连接高并发接入
- 采用 C++17 为主工程标准，内置 C++20 无栈协程模块，切换开销更小、并发能力更强
- 自研多线程协程调度器，支持多核 Worker 均衡调度，充分利用多核 CPU 资源
- 业务端口与监控端口隔离，业务流量和运维监控流量互不干扰
- CMake 模块化工程架构，按功能拆分为独立静态库，模块解耦、易维护、易扩展
- 实现服务优雅退出，捕获系统信号平滑关闭事件循环、释放连接与资源
- 接入 Prometheus 监控指标，覆盖在线连接数、QPS、请求延迟，支持可视化运维
- 完整 Benchmark 基准压测体系，量化不同并发模型性能差异
- 支持 Docker Compose 统一编排中间件，实现开发、演示、部署环境一致性

## 性能优化・数据对比（高并发亮点）
### 优化前
- 接口 QPS：3000
- 核心瓶颈：频繁内存拷贝、系统调用冗余；协程调度依赖互斥锁，高并发锁竞争严重；线程上下文切换开销大，CPU 利用率低，高并发场景存在性能抖动。

### 优化后
- 接口 QPS：7000+
- 吞吐提升：133.33%
- 延迟表现：P99 请求延迟显著降低，响应更平稳
- 稳定性：高并发压力下服务无阻塞、无抖动、CPU 资源利用率合理

### 核心优化手段
- 网络层采用 writev 向量 IO 零拷贝机制，聚合报文头与包体批量发送，减少用户态与内核态数据拷贝及系统调用次数，大幅提升网络吞吐。
- 协程调度采用 Lock-Free 无锁队列，基于原子操作替代互斥锁，彻底消除高并发下锁竞争与线程阻塞问题。
- 基于 epoll 多路复用 + 固定线程池模型，避免线程频繁创建销毁，降低上下文切换开销。
- 工程层面模块化拆分、编译依赖规整，提升服务运行稳定性与迭代扩展性。

## 后续计划
- 持续迭代协程调度策略，进一步提升高并发场景下吞吐与延迟表现
- 完善各模块单元测试，提升项目测试覆盖率
- 接入 CI/CD 流程，实现代码提交自动编译、构建与基础校验
- 支持 TLS/SSL 加密通信，提升网络传输安全性
- 强化限流、熔断、降级策略，完善微服务高可用体系
- 优化长连接断线重连与会话保持机制
- 完善监控大盘配置与告警规则，适配线上生产运维标准
