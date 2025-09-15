#include <iostream>
#include <vector>
#include <string>

struct Task {
    int id;
    std::string description;

    // Constructor
    Task(int i, std::string d) : id(i), description(std::move(d)) {
        std::cout << "✅ Constructed Task(" << id << ", " << description << ")\n";
    }

    // Copy constructor
    Task(const Task& other) : id(other.id), description(other.description) {
        std::cout << "📋 Copied Task(" << id << ", " << description << ")\n";
    }

    // Move constructor
    Task(Task&& other) noexcept : id(other.id), description(std::move(other.description)) {
        std::cout << "🚚 Moved Task(" << id << ", " << description << ")\n";
    }
};

int main() {
    std::vector<Task> tasks;

//    std::cout << "\n--- Using push_back() ---\n";
//    tasks.push_back(Task{1, "Breakfast"}); // Construct temporary + move into vector

    std::cout << "\n--- Using emplace_back() ---\n";
    tasks.emplace_back(2, "Lunch"); // Construct directly inside vector (no move)

    std::cout << "\n--- Done ---\n";
    return 0;
}
