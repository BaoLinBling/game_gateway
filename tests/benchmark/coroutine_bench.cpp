#include <iostream>
#include <chrono>
#include <vector>
#include <coroutine>

struct SimpleTask {
    struct promise_type {
        SimpleTask get_return_object() {
            return {};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

SimpleTask foo() {
    co_return;
}

int main() {
    const int N = 10000;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        foo();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto cost = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "cpp20_coroutine_us=" << cost << std::endl;
    std::cout << "type,ops,time_us\n";
    std::cout << "cpp20," << N << "," << cost << "\n";

    return 0;
}
