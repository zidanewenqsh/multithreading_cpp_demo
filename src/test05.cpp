#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
void func(std::promise<int> pro) {
    pro.set_value(100);  
}

int main() {
    // 计时
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::promise<int> pro;
    std::future<int> result = pro.get_future();
    std::thread t(func, std::move(pro));
    t.join();
    if (result.valid()) {
        std::cout << result.get() << std::endl;
    }


    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    return 0;
}