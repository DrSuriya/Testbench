#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "task.h"
#include <algorithm>
#include <vector>
#include <mutex>
#include <sstream>

// Class to manage a collection of tasks
class taskManager {
private:
    std::vector<task> tasks; // Vector to store tasks
    std::mutex mtx; // Mutex to ensure thread safety

public:
    // Method to add a task to the task manager
    void addTask(const task &t);

    // Method to get the list of tasks
    std::vector<task>& getTasks(); // Ensure this method is declared properly
};

#endif
