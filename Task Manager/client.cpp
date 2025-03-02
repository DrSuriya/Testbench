#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

#define PORT 2002

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Read tasks from file
    std::ifstream file("tasks.txt");
    std::string tasks, line;
    while (getline(file, line)) {
        tasks += line + ";"; // Append each line from the file to the tasks string, separated by semicolons
    }
    file.close();

    // Send tasks to the server
    sendto(sockfd, tasks.c_str(), tasks.size(), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    std::cout << "Sent tasks: " << tasks << "\n";

    // Receive updates from server
    while (recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL) > 0) {
        std::cout << "Server Update: " << buffer << "\n"; // Print server updates
    }

    // Close the socket
    close(sockfd);
    return 0;
}
