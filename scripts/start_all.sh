#!/bin/bash

echo "[start] game_gateway"

cd $(dirname "$0")/../build

./game_gateway > gateway.log 2>&1 &
echo $! > gateway.pid

# 等待服务启动
sleep 2

echo "[start] done"
