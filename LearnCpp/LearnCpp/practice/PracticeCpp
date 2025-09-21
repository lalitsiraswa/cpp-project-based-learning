#include <iostream>
#include <vector>
#include <string>

struct Task {
    int id;
    std::string description;
    bool completed = false;  // default member initializer

    // Constructor
    Task(int i, std::string d, bool c = false)
        : id(i), description(std::move(d)), completed(c) {
            std::cout << "Constructed Task(" << id << ", " << description << ", " << completed << std::endl;
        }
};

int main() {
    // 1️⃣ Basic type initialization
    int a{10};      // ✅ value-initialized to 10
    int b{};        // ✅ value-initialized to 0 (instead of garbage!)
    double c{3.14}; // ✅ value-initialized to 3.14

    std::cout << "a=" << a << " b=" << b << " c=" << c << "\n";

    // 2️⃣ Struct initialization
    Task t1{1, "Breakfast"};          // ✅ brace-initialization
    Task t2{2, "Lunch", true};        // ✅ all fields initialized
    Task t3{3, "Dinner"};             // ✅ completed uses default false

    std::cout << "t1: " << t1.description << " completed=" << t1.completed << "\n";
    std::cout << "t2: " << t2.description << " completed=" << t2.completed << "\n";
    std::cout << "t3: " << t3.description << " completed=" << t3.completed << "\n";

    // 3️⃣ Initializing an array
    int numbers[]{1, 2, 3, 4, 5};  // ✅ array initialized
    std::cout << "numbers[2] = " << numbers[2] << "\n";

    // 4️⃣ Initializing a vector
    std::vector<int> vec{10, 20, 30}; // ✅ fills with 10, 20, 30
    std::cout << "vec size = " << vec.size() << "\n";

    // 5️⃣ Avoiding narrowing conversion
    // int x{3.14};  // ❌ Compile error: narrowing conversion (safe!)
    int x(3.14);     // ⚠️ Allowed in old-style initialization (value will be truncated)
    std::cout << "x = " << x << "\n";

    return 0;
}
