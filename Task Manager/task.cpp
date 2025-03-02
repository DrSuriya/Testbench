#include "task.h"

// Constructor for the task class
task::task(std::string name, int priority) {
    this->name = name; // Set the task name
    this->priority = priority; // Set the task priority
    taskStatus = status::Pending; // Initialize task status to Pending
}

// Method to execute the task
std::string task::execute() {
    taskStatus = status::Running; // Set task status to Running
    std::cout << "Executing Task: " << name << "...\n"; // Print executing message
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate task execution with a 2-second delay
    taskStatus = status::Completed; // Set task status to Completed
    
    std::string result = "Task " + name + " completed."; // Create result message
    std::cout << result << "\n"; // Print result message

    // Log the result to a file
    std::ofstream logFile("task_log.txt", std::ios::app); // Open log file in append mode
    logFile << result << "\n"; // Write result to log file
    logFile.close(); // Close the log file

    return result; // Return the result message
}
