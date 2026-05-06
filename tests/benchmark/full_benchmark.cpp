#include <iostream>
#include <chrono>
#include <thread>
#include<fstream>
#include "coroutine_cpp20/advanced_scheduler.h"

using namespace coroutine_cpp20;

void fakeWork() {
    volatile int x = 0;
    for (int i = 0; i < 100; i++) x += i;
}

int runSchedulerTest() {
    auto& s = AdvancedScheduler::instance();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000; ++i) {
        s.schedule(fakeWork);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    int us = runSchedulerTest();

    std::cout << "type,ops,time_us\n";
    std::cout << "cpp20_scheduler,10000," << us << "\n";
    //写入
    const char* filepath = "/home/blingbling/projects/game_gateway/bench_cpp20.csv";
    //追加
    std::ofstream f(filepath, std::ios::app);
    //std::ofstream f("bench_cpp20.csv", std::ios::app);
    // 如果文件是空的，自动写入表头（只执行一次）
    if (f.is_open() && f.tellp() == 0) {
        f << "type,ops,time_us\n";
    }

    if (f.is_open()) {
        f << "cpp20_scheduler,10000," << us << std::endl;
        f.close();
    } else {
        std::cerr << "\n错误：无法打开文件！" << std::endl;
    }

    return 0;
}
