#ifndef TASK_H
#define TASK_H

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>

// Enumeration to represent the status of a task
enum class status {
    Pending,    // Task is pending
    Running,    // Task is currently running
    Completed   // Task has been completed
};

// Class to represent a task
class task {
public:
    std::string name;    // Name of the task
    int priority;        // Priority of the task
    status taskStatus;   // Current status of the task

    // Constructor to initialize a task with a name and priority
    task(std::string name, int priority);

    // Method to execute the task
    std::string execute(); // Ensure this method is correctly declared
};

#endif
