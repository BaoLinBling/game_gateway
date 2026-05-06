#pragma once
#include <boost/lockfree/queue.hpp>

template<typename T>
class LockFreeQueue {
public:
    LockFreeQueue(size_t size = 1024) : q_(size) {}

    bool push(const T& v) {
        return q_.push(v);
    }

    bool pop(T& v) {
        return q_.pop(v);
    }

private:
    boost::lockfree::queue<T> q_;
};
