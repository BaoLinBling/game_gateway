#!/bin/bash

echo "[stress] start 1000 concurrency test"

cd $(dirname "$0")
python3 ../tests/e2e/e2e_concurrency.py

echo "[stress] done"
