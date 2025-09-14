#pragma once
#include "Task.hpp"
#include <vector>
#include <string>
#include <mutex>

class TaskManager {
public:
    explicit TaskManager(std::string file);
    ~TaskManager();

    void addTask(const std::string& desc, Priority prio);
    void completeTask(int id);
    void listTasks() const;
    int countPendingTasks() const;

private:
    void saveUnlocked(std::ofstream& out) const; // helper that does NOT lock
    void save() const;                            // locks and writes
    void load();

private:
    mutable std::mutex mtx;
    std::vector<Task> tasks;
    std::string filename;
    int nextId = 1;
};
