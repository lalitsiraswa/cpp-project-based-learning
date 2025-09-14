#include "Task.hpp"
#include <sstream>

std::string Task::priorityToString() const {
    switch (priority) {
        case Priority::Low: return "Low";
        case Priority::Medium: return "Medium";
        case Priority::High: return "High";
    }
    return "Unknown";
}

Priority Task::stringToPriority(const std::string& s) {
    if (s == "Low") return Priority::Low;
    if (s == "Medium") return Priority::Medium;
    return Priority::High;
}

std::string Task::serialize() const {
    // Format: id|description|priority|completed
    // Note: description must not contain newline. Simple escaping of '|' would be needed for production.
    std::ostringstream oss;
    oss << id << "|" << description << "|" << priorityToString() << "|" << (completed ? "1" : "0");
    return oss.str();
}

Task Task::deserialize(const std::string& line) {
    Task t;
    std::istringstream iss(line);
    std::string part;
    int field = 0;
    while (std::getline(iss, part, '|')) {
        switch (field) {
            case 0: try { t.id = std::stoi(part); } catch (...) { t.id = 0; } break;
            case 1: t.description = part; break;
            case 2: t.priority = stringToPriority(part); break;
            case 3: t.completed = (part == "1"); break;
            default: break;
        }
        ++field;
    }
    return t;
}
