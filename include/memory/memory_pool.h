#pragma once
#include <cstddef>

namespace memory {

// 初始化内存池（建议：传入 1GB）
void mp_init(size_t size);

// 分配内存
void* mp_alloc(size_t size);

// 释放内存
void mp_free(void* ptr);

}
