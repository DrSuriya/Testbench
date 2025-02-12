#include "taskManager.h"

// Method to add a task to the task manager
void taskManager::addTask(const task &t) {
    // Lock the mutex to ensure thread safety
    std::lock_guard<std::mutex> lock(mtx);
    // Add the task to the list of tasks
    tasks.push_back(t);
}

// Method to get the list of tasks
// Ensure this method is correctly defined
std::vector<task>& taskManager::getTasks() {
    // Return the list of tasks
    return tasks;
}
