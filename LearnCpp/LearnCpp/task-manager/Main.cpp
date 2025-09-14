#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <limits>
#include "TaskManager.hpp"

std::condition_variable cv;
std::mutex cvMtx; // just for the condition variable wait

void reminderThread(const TaskManager& tm, const std::atomic<bool>& stopFlag) {
    using namespace std::chrono_literals;
    std::unique_lock lock(cvMtx);
    while (!stopFlag.load()) {
        // wait for 30s OR until stopFlag changes
        if (cv.wait_for(lock, 30s, [&]{ return stopFlag.load(); }))
            break; // stopFlag triggered

        int pending = tm.countPendingTasks();
        if (pending > 0) {
            std::cout << "\n[Reminder] You have " << pending
                      << " pending task(s).\n";
        }
    }
}

int main() {
    namespace fs = std::filesystem;
    std::cout << "Current working directory: " << fs::current_path() << "\n";
    TaskManager tm("todo.txt");
    std::atomic<bool> stopFlag{false};

    // Start notifier thread
    std::thread notifier(reminderThread, std::cref(tm), std::cref(stopFlag));

    while (true) {
        std::cout << "\n1. Add Task\n2. Complete Task\n3. List Tasks\n4. Exit\nChoice: ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            // EOF or error -> exit
            break;
        }
        if (input.empty()) continue;

        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input, please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            std::cout << "Enter description: ";
            std::string desc;
            std::getline(std::cin, desc);
            if (desc.empty()) {
                std::cout << "Empty description; task not added.\n";
                continue;
            }
            tm.addTask(desc, Priority::Medium);
            std::cout << "Added: " << desc << "\n";
        }
        else if (choice == 2) {
            std::cout << "Enter task id: ";
            std::string idStr;
            std::getline(std::cin, idStr);
            int id = 0;
            try {
                id = std::stoi(idStr);
            } catch (...) {
                std::cout << "Invalid id. Try again.\n";
                continue;
            }
            tm.completeTask(id);
        }
        else if (choice == 3) {
            tm.listTasks();
        }
        else if (choice == 4) {
            stopFlag.store(true);
            cv.notify_all();  // wake up the sleeping thread immediately
            break;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    // signal the reminder thread to stop and join it
    stopFlag.store(true);
    if (notifier.joinable()) notifier.join();

    std::cout << "Exiting...\n";
    return 0;
}
