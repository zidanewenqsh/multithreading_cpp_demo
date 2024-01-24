#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <memory>
#include <future>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <queue>

class ThrdPool {
public:
    ThrdPool(int threadnum):stop(false) {
        std::cout << "ThrdPool::ThrdPool()\n";
        for (int i = 0; i < threadnum; ++i) {
            thrd_pool.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> l(this->mtx);
                        cv.wait(l, [this]{return this->stop || !this->tasks.empty();});
                        if (this->stop && this->tasks.empty()) return ;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThrdPool() {
        std::cout << "ThrdPool::~ThrdPool()\n";
        {
            std::unique_lock<std::mutex> lk(mtx);
            stop = true;
            cv.notify_all();
        }
        for (auto& thrd : thrd_pool) {
            if (thrd.joinable()) thrd.join();
        }
    }
    
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using ret_type = decltype(f(args...));
        // std::function<void()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::function<ret_type()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::shared_ptr<std::packaged_task<ret_type()>> task_ptr = std::make_shared<std::packaged_task<ret_type()>>(func);
        std::future<ret_type> res = task_ptr->get_future();
        {
            std::unique_lock<std::mutex> l(this->mtx);
            this->tasks.emplace([task_ptr](){(*task_ptr)();});
        }
        this->cv.notify_one();
        return res;
    }



private:
    std::vector<std::thread> thrd_pool;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};

int main() {
    ThrdPool pool(2);

    auto startTime = std::chrono::high_resolution_clock::now();

    auto printTimeDiff = [startTime](const std::string& taskName) {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
        std::cout << taskName << " completed at " << duration << " seconds." << std::endl;
    };

    auto task1 = pool.enqueue([printTimeDiff]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        printTimeDiff("Task 1");
    });

    auto task2 = pool.enqueue([printTimeDiff]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printTimeDiff("Task 2");
    });

    auto task3 = pool.enqueue([printTimeDiff]() {
        printTimeDiff("Task 3");
    });

    auto task4 = pool.enqueue([printTimeDiff]() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        printTimeDiff("Task 4");
    });

    task1.get();
    task2.get();
    task3.get();
    task4.get();

    // 添加一个有参数和返回值的任务
    auto sumTask = pool.enqueue([](int a, int b) -> int {
        return a + b;
    }, 5, 10); // 计算 5 + 10

    // 获取任务结果
    int sumResult = sumTask.get();
    std::cout << "Sum task result: " << sumResult << std::endl; // 输出计算结果

    std::cout << "All tasks completed." << std::endl;


    return 0;
}
