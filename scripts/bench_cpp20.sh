#!/bin/bash
set -e

echo "[BENCH] build C++20 coroutine benchmark..."

g++ -std=c++20 -O2 \
  tests/benchmark/coroutine_bench.cpp \
  -o coroutine_bench

echo "[BENCH] running..."
./coroutine_bench

# 修复：自动把输出重定向到 csv 文件
./coroutine_bench | grep -E 'cpp20|type' > cpp20_bench.csv

echo "[BENCH] CSV result:"
cat cpp20_bench.csv
