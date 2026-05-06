#include <gtest/gtest.h>
#include "memory/memory_pool.h"
#include <vector>
#include <thread>

using namespace memory;

TEST(MemoryTest, BasicAllocFree) {
    mp_init(1024ULL * 1024 * 1024);

    void* p1 = mp_alloc(64);
    void* p2 = mp_alloc(128);

    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);

    mp_free(p1);
    mp_free(p2);
}

TEST(MemoryTest, MultipleAlloc) {
    mp_init(1024ULL * 1024 * 1024);

    std::vector<void*> vec;

    for (int i = 0; i < 100; ++i) {
        void* p = mp_alloc(256);
        if (p) vec.push_back(p);
    }

    ASSERT_GT(vec.size(), 0);

    for (auto p : vec) {
        mp_free(p);
    }
}

TEST(MemoryTest, MultiThread) {
    mp_init(1024ULL * 1024 * 1024);

    std::vector<std::thread> threads;

    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([]() {
            for (int j = 0; j < 1000; ++j) {
                void* p = mp_alloc(128);
                if (p) {
                    mp_free(p);
                }
            }
        });
    }

    for (auto& t : threads) t.join();

    SUCCEED();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
