# Task Manager - Client-Server System

## Project Overview
This project implements a **real-time, concurrent task execution system** using a **client-server architecture**. The **client** reads tasks from a file and sends them to the **server** over **UDP sockets**. The **server** receives, prioritizes, executes the tasks, and returns execution results to the client.

### **Key Features**
✔ **Client-Server Communication**: Uses UDP sockets for lightweight, fast data transmission.  
✔ **Task Scheduling & Execution**: Tasks are **sorted by priority** and executed sequentially.  
✔ **Concurrency**: The server **handles multiple clients simultaneously** using threads.  
✔ **Logging**: Task execution results are saved in `task_log.txt`.  

---

## **Setup**
### **1️. Prerequisites**
- **g++ (GCC) compiler**
- **Linux/macOS terminal or WSL (Windows Subsystem for Linux)**

### **2️. Clone the Repository**
```sh
git clone https://github.com/your-repository/task-manager.git
cd task-manager
```

### **3️. Compile the Code**
```sh
g++ server.cpp taskManager.cpp task.cpp -o server -pthread
g++ client.cpp -o client
```

---

## **Usage**
### **1️. Create a Task List (`tasks.txt`)**
Before running the client, create a `tasks.txt` file:
```txt
Implement authentication system,3
Optimize database queries,5
Fix login bug,1
Update API documentation,2
Plan sprint meeting,4
```

### **2. Start the Server**
Run the server first:
```sh
./server
```
- The server will **wait for tasks from the client**.

### **3. Run the Client**
Run the client to **send tasks to the server**:
```sh
./client
```
- The client **reads `tasks.txt`**, sends it to the **server**, and **receives execution updates**.

---

## **Expected Output**
### **On the Server:**
```
Server waiting for tasks...
Received Task List: Implement authentication system,3;Optimize database queries,5;Fix login bug,1;Update API documentation,2;Plan sprint meeting,4;
Tasks added. Sorting and executing...
Executing Task: Optimize database queries...
Task Optimize database queries completed.
Executing Task: Plan sprint meeting...
Task Plan sprint meeting completed.
Executing Task: Implement authentication system...
Task Implement authentication system completed.
Executing Task: Update API documentation...
Task Update API documentation completed.
Executing Task: Fix login bug...
Task Fix login bug completed.
```

### **On the Client:**
```
Sent tasks: Implement authentication system,3;Optimize database queries,5;Fix login bug,1;Update API documentation,2;Plan sprint meeting,4;
Server Update: Executing: Optimize database queries
Server Update: Executing: Plan sprint meeting
Server Update: Executing: Implement authentication system
Server Update: Executing: Update API documentation
Server Update: Executing: Fix login bug
Task Optimize database queries completed.
Task Plan sprint meeting completed.
Task Implement authentication system completed.
Task Update API documentation completed.
Task Fix login bug completed.
```

---

## **How It Works**
1. **Client**
   - Reads **tasks from `tasks.txt`**.
   - Sends the **formatted task list** to the server via **UDP sockets**.
   - Receives **execution updates** from the server.
   - Displays **task execution results**.

2. **Server**
   - Waits for **task requests from clients**.
   - Receives **task list** and **adds them to the task manager**.
   - **Sorts tasks by priority** (highest priority first).
   - **Executes tasks** sequentially in **worker threads**.
   - Sends execution updates **back to the client**.

---

## 📂 **Project Structure**
```
📂 Task-Manager
🌟 client.cpp           # Sends task list to server, receives execution updates
🌟 server.cpp           # Receives tasks, executes them, sends results back
🌟 task.h               # Task class definition
🌟 task.cpp             # Task implementation (execution, logging)
🌟 taskManager.h        # Task Manager class definition
🌟 taskManager.cpp      # Task Manager implementation (sorting, execution)
🌟 tasks.txt            # Task list input (created by the user)
🌟 task_log.txt         # Task execution logs
🌟 README.md            # Project documentation
```

