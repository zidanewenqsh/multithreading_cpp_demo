#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
int func(int x) {
    int ret = 0;
    for (int i = x; i < 10000; i++) {
        ret++;
    }
    return ret;
}

int main() {
    // 计时
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    int arg = 100;
    std::packaged_task<int(int)> task(func);
    std::future<int> result = task.get_future();
    std::thread t(std::move(task), arg);
    t.join();
    if (result.valid()) {
        std::cout << result.get() << std::endl;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    return 0;
}