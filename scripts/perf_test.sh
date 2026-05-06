#!/bin/bash

echo "=============================="
echo " Game Gateway Perf Test"
echo "=============================="

cd "$(dirname "$0")/.."

HOST=127.0.0.1
PORT=8080
CONCURRENCY=200
TOTAL=20000

echo "[info] concurrency=$CONCURRENCY, total=$TOTAL"

python3 tests/e2e/e2e_concurrency.py

echo "[done] performance test finished"
