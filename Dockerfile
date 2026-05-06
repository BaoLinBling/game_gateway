FROM ubuntu:22.04

# 安装运行依赖
RUN apt update && apt install -y \
    libspdlog-dev \
    libhiredis-dev \
    librdkafka-dev \
    libyaml-cpp-dev \
    libprotobuf-dev \
    libmysqlclient-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
