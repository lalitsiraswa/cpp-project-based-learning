#pragma once
#include <string>
#include <optional>
#include <chrono>

enum class Priority { Low, Medium, High };

struct Task {
    int id{};
    std::string description;
    Priority priority{Priority::Low};
    std::optional<std::chrono::system_clock::time_point> dueDate;
    bool completed{false};

    [[nodiscard]] std::string priorityToString() const;
    static Priority stringToPriority(const std::string& s);

    [[nodiscard]] std::string serialize() const;
    static Task deserialize(const std::string& line);
};
