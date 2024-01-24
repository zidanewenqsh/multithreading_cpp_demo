#include <future>
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <future>
std:: condition_variable cv_;
std::mutex mutex;
bool done = false;
#define maxsize 5

struct Job {
    std::shared_ptr<std::promise<std::string>> pro;
    std::string input;
};
std::queue<Job> que;

void commit() {
    std::cout << "commit" << std::endl;
    int i = 0;
    while (true) {
        Job job;
        std::future<std::string> result_future;
        {
            std::unique_lock<std::mutex> l(mutex);
            std::cout << "commit " << i << ", que.size():" << que.size() <<std::endl;
            cv_.wait(l, [&]{ return que.size() < maxsize; }); // 等待队列不满
            job.pro = std::make_shared<std::promise<std::string>>();
            // job.pro.reset(new std::promise<std::string>);
            job.input = std::to_string(i++);
            result_future = job.pro->get_future();
            // que.push(std::to_string(i++));
            que.push(job);
            cv_.notify_one();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // 获取处理结果
        std::string processed_result = result_future.get();
        std::cout << "Worker processed: " << processed_result << std::endl;
    }
}

void worker() {
    std::cout << "worker" << std::endl;
    
    while (!done) {
        Job job;
        {
            std::unique_lock<std::mutex> l(mutex);
            cv_.wait(l, [&] { return !que.empty() || done; });
            if (done) {
                return;
            }
            job = que.front();
            que.pop();
        }
        std::string result = "Process:" + job.input;
        job.pro->set_value(result);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

int main() {
    std::thread commit_thread(commit);
    std::thread worker_thread(worker);
    commit_thread.join();
    worker_thread.join();
    return 0;
}