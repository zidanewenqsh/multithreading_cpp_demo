#include <iostream>
#include <future>
#include <thread>
int main() {
    std::packaged_task<int(int, int)> task([](int a, int b) {
        return a + b;
    });
    std::future<int> result = task.get_future();
    
    // 执行任务
    // task(5, 3);
    std::thread t(std::move(task), 5, 3);
    t.join();
    // 等待结果并处理
    int sum = result.get();  // 阻塞等待结果
    std::cout << "The sum is: " << sum << std::endl;

    return 0;
}



// #include <iostream>
// #include <future>

// int main() {
//     std::packaged_task<int(int, int)> task([](int a, int b) {
//         return a + b;
//     });
//     std::future<int> result = task.get_future();
//     task(5, 3); // 正确：第一次调用
    
//     // 等待结果并处理

//     int sum = result.get();  // 阻塞等待结果

//     std::cout << "The sum is: " << sum << std::endl;

//     return 0;   
// }
//     // 再次尝试调用
//     // try {
//     //     task(10, 20); // 错误：将抛出异常
//     // } catch (const std::future_error& e) {
//     //     std::cout << "Exception: " << e.what() << '\n';
//     // }