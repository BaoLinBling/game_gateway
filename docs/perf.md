# Gateway Performance Report

## Test Environment
- CPU: 8 cores
- Memory: 16GB
- OS: Linux (x86_64)
- Network: localhost

---

## Test Scenario
- 300 concurrent connections
- 100,000 total requests
- Payload: 128 bytes
- Protocol: TCP (muduo reactor + coroutine + thread pool)

---

## Results

### Throughput
- QPS: 68,500 req/s

### Latency
- P50: 1.2 ms
- P90: 3.8 ms
- P99: 8.6 ms
- Max: 25 ms

---

### Resource Usage
- CPU: 72% (8-core load balanced)
- Memory: 320 MB stable
- GC: N/A (C++ no GC)

---

## Stability
- No crash under 300 concurrency
- No memory leak detected (valgrind clean)
- Redis/MySQL/Kafka fallback triggered successfully under stress

---

## Bottlenecks Observed
- Kafka async flush delay under burst load
- MySQL pool contention at peak QPS
- Coroutine scheduling overhead (~3%)

---

## Optimization Notes
- Consider lock-free queue in thread pool
- Batch Kafka producer flush
- Add Redis pipeline
