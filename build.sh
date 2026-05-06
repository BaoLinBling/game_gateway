#!/bin/bash

USE_CPP20=0

if [[ "$1" == "--cpp20" ]]; then
  USE_CPP20=1
fi

if [[ $USE_CPP20 -eq 1 ]]; then
  echo "[BUILD] 启用 C++20 协程模式"
  CXXFLAGS="-std=gnu++20 -O2"
else
  echo "[BUILD] 默认 C++17 模式"
  CXXFLAGS="-std=gnu++17 -O2"
fi

mkdir -p build
cd build
cmake .. -DCMAKE_CXX_FLAGS="$CXXFLAGS"
make -j$(nproc)
