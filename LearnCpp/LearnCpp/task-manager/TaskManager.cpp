#include "TaskManager.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;

TaskManager::TaskManager(std::string file) : filename(std::move(file)) {
    load();
}

TaskManager::~TaskManager() {
    // ensure final save (if needed) - lock to be safe
    std::lock_guard lock(mtx);
    std::ofstream out(filename, std::ios::trunc);
    saveUnlocked(out);
}

// Helper: write tasks to already-opened stream (no mutex inside)
void TaskManager::saveUnlocked(std::ofstream& out) const {
    for (const auto& t : tasks) {
        out << t.serialize() << "\n";
    }
    out.flush();
}

// Public save that locks and writes atomically via temp file
void TaskManager::save() const {
    std::lock_guard lock(mtx);
    // write to temp file then atomically replace
    fs::path tmp = filename;
    tmp += ".tmp";

    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out.is_open()) {
            std::cerr << "Failed to open temp file for saving: " << tmp << "\n";
            return;
        }
        saveUnlocked(out);
    }
    // rename (atomic on most OS)
    std::error_code ec;
    fs::rename(tmp, filename, ec);
    if (ec) {
        std::cerr << "Failed to replace data file: " << ec.message() << "\n";
    }
}

void TaskManager::addTask(const std::string& desc, Priority prio) {
    std::lock_guard lock(mtx);
    tasks.push_back(Task{nextId++, desc, prio});
    // write while still holding lock but using helper to avoid re-lock
    std::ofstream out(filename, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << "\n";
        return;
    }
    saveUnlocked(out);
}

void TaskManager::completeTask(int id) {
    std::lock_guard lock(mtx);
    for (auto &t : tasks) {
        if (t.id == id) {
            t.completed = true;
            // save while holding lock
            std::ofstream out(filename, std::ios::trunc);
            if (!out.is_open()) {
                std::cerr << "Failed to open file for saving: " << filename << "\n";
                return;
            }
            saveUnlocked(out);
            return;
        }
    }
    // if not found: optionally print message (not locking-free)
    std::cerr << "Task id " << id << " not found.\n";
}

void TaskManager::listTasks() const {
    std::lock_guard lock(mtx);
    // Use ranges to filter pending tasks (modern C++)
    for (const auto &t : tasks | std::views::filter([](const Task& t){ return !t.completed; })) {
        std::cout << t.id << ". " << t.description << " [" << t.priorityToString() << "]\n";
    }
}

int TaskManager::countPendingTasks() const {
    std::lock_guard lock(mtx);
    return static_cast<int>(std::count_if(tasks.begin(), tasks.end(), [](const Task& t){ return !t.completed; }));
}

void TaskManager::load() {
    // load without locking; ctor is single-threaded typically
    std::ifstream in(filename);
    if (!in.is_open()) {
        // no file yet — that's fine
        return;
    }
    tasks.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        tasks.push_back(Task::deserialize(line));
    }
    if (!tasks.empty()) nextId = tasks.back().id + 1;
}
