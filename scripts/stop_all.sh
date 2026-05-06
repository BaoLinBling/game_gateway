#!/bin/bash

echo "[stop] game_gateway"

cd $(dirname "$0")

if [ -f ../build/gateway.pid ]; then
    kill -9 $(cat ../build/gateway.pid)
    rm ../build/gateway.pid
fi

echo "[stop] done"
