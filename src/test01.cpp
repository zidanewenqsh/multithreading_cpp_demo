#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <mutex>
int share_data = 0;
std::mutex mtx;
void func() {
    for (int i = 0; i < 100000000; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        share_data++;
    }
}

int main() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::thread t1(func);
    std::thread t2(func);
    t1.join();
    t2.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << share_data << std::endl;
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    return 0;
}