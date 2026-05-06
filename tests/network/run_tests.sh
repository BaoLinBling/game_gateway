#!/usr/bin/env bash
set -e
echo "=============================="
echo " Running Network Tests"
echo "=============================="
cd build
./network_tests
echo "=============================="
echo " ALL TESTS PASSED ✅"
echo "=============================="
