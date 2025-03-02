#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "taskManager.h"

#define PORT 2002

taskManager manager;

// Function to handle client requests
void handleClient(int sockfd, struct sockaddr_in cliaddr, std::string taskList) {
    socklen_t len = sizeof(cliaddr);
    std::string results;

    size_t pos = 0;
    // Parse the task list received from the client
    while ((pos = taskList.find(";")) != std::string::npos) {
        std::string taskInfo = taskList.substr(0, pos);
        size_t commaPos = taskInfo.find(",");

        if (commaPos != std::string::npos) {
            std::string name = taskInfo.substr(0, commaPos);
            try {
                int priority = std::stoi(taskInfo.substr(commaPos + 1));
                manager.addTask(task(name, priority));
            } catch (...) {
                std::cerr << "Invalid priority value for task: " << name << "\n";
            }
        }
        taskList.erase(0, pos + 1);
    }

    std::cout << "Tasks added. Sorting and executing...\n";

    // Execute tasks and send updates to the client
    for (auto &t : manager.getTasks()) {
        if (t.taskStatus == status::Pending) {
            std::string update = "Executing: " + t.name;
            sendto(sockfd, update.c_str(), update.size(), 0, (struct sockaddr*)&cliaddr, len);
            results += t.execute() + "\n";
        }
    }

    // Send the results back to the client
    sendto(sockfd, results.c_str(), results.size(), 0, (struct sockaddr*)&cliaddr, len);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Initialize server address structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the server address
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    std::cout << "Server waiting for tasks...\n";

    while (true) {
        socklen_t len = sizeof(cliaddr);

        // Receive task list from client
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);
        std::cout << "Received Task List: " << buffer << "\n";

        // Handle client request in a separate thread
        std::thread worker(handleClient, sockfd, cliaddr, std::string(buffer));
        worker.detach();
    }

    // Close the socket
    close(sockfd);
    return 0;
}
