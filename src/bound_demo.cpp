#include <iostream>
#include <functional>

void print(int a, int b) {
    std::cout << a << ", " << b << std::endl;
}

class MyClass {
public:
    void memberFunc(int x) {
        std::cout << "Value: " << x << std::endl;
    }
};
int main() {
    // 绑定自由函数
    std::function<void()> bound_print = std::bind(print, 10, 20);
    bound_print(); // 输出: 10, 20

    // 绑定成员函数
    MyClass obj;
    std::function<void()> bound_memberFunc = std::bind(&MyClass::memberFunc, &obj, 42);
    bound_memberFunc(); // 输出: Value: 42

    // 使用占位符
    using namespace std::placeholders;  // 需要这个命名空间来使用占位符
    std::function<void(int, int)> bound_print_with_placeholder = std::bind(print, _1, _2);
    bound_print_with_placeholder(30, 40); // 输出: 30, 40

    return 0;
}

