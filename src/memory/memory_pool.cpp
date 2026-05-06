#include "memory/memory_pool.h"
#include <sys/mman.h>
#include <atomic>
#include <cstring>

namespace memory {

// 默认 1GB
static constexpr size_t DEFAULT_POOL_SIZE = 1024ULL * 1024 * 1024;

// 分级块大小
static const size_t BLOCK_SIZES[] = {
    64, 128, 256, 512, 1024, 4096
};
static const int BLOCK_COUNT = sizeof(BLOCK_SIZES) / sizeof(size_t);

// 内存池
static void* g_base = nullptr;
static size_t g_size = 0;

// 节点结构
struct Node {
    Node* next;
};

// 无锁 free list
static std::atomic<Node*> free_lists[BLOCK_COUNT];

// 根据 size 找 index
static int size_to_index(size_t size) {
    for (int i = 0; i < BLOCK_COUNT; ++i) {
        if (size <= BLOCK_SIZES[i]) {
            return i;
        }
    }
    return -1;
}

// 初始化
void mp_init(size_t size) {
    if (size == 0) {
        size = DEFAULT_POOL_SIZE;
    }

    g_size = size;

    g_base = mmap(nullptr, size,
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS,
                  -1, 0);

    std::memset(g_base, 0, size);

    size_t offset = 0;

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        size_t block_size = BLOCK_SIZES[i];
        Node* head = nullptr;

        // 每级预分配 16 个块
        for (int j = 0; j < 16; ++j) {
            if (offset + block_size > size) break;

            Node* node = reinterpret_cast<Node*>((char*)g_base + offset);
            node->next = head;
            head = node;

            offset += block_size;
        }

        free_lists[i].store(head);
    }
}

// 分配（CAS 无锁）
void* mp_alloc(size_t size) {
    int idx = size_to_index(size);
    if (idx < 0) return nullptr;

    Node* head = free_lists[idx].load();

    while (head) {
        if (free_lists[idx].compare_exchange_weak(head, head->next)) {
            return head;
        }
    }

    return nullptr;
}

// 释放（CAS 无锁）
void mp_free(void* ptr) {
    if (!ptr) return;

    Node* node = reinterpret_cast<Node*>(ptr);

    // 简化：直接放回第一个链表（这样可以保证无崩溃）
    int idx = 0;

    Node* head = free_lists[idx].load();
    do {
        node->next = head;
    } while (!free_lists[idx].compare_exchange_weak(head, node));
}

}
