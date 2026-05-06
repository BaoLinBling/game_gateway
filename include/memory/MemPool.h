#pragma once
#include <cstdlib>
#include <muduo/base/Mutex.h>

#define MEM_POOL_SIZE 4096

class MemPool {
public:
    static MemPool& instance() {
        static MemPool pool;
        return pool;
    }

    void* malloc(size_t sz) {
        if (sz > MEM_POOL_SIZE) return ::malloc(sz);
        muduo::MutexLockGuard lock(mutex_);
        if (!free_) {
            free_ = ::malloc(MEM_POOL_SIZE);
        }
        void* p = free_;
        free_ = nullptr;
        return p;
    }

    void free(void* p) {
        muduo::MutexLockGuard lock(mutex_);
        free_ = p;
    }

private:
    MemPool() = default;
    muduo::MutexLock mutex_;
    void* free_ = nullptr;
};

inline void* operator new(size_t sz) { return MemPool::instance().malloc(sz); }
inline void  operator delete(void* p) { MemPool::instance().free(p); }
