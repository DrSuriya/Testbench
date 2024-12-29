/**
 * Author: Hasan Suriya - 101193772
 * Co-Author: Muhammad Maisam - 101151910
 * Course: SYSC 4001 - Operating Systems
 * Assignment: Assignment 1 - Design and Implementation of an Interrupt Simulator
 * Date: 6-12-2024
 *
 * Description:
 * This file contains the implementation of an interrupt simulator for the SYSC 4001 - Operating Systems course.
 *
 * The simulator supports three scheduling algorithms: First-Come, First-Served (FCFS), Earliest
 * Priority (EP), and Round Robin (RR).
 *
 * The simulator reads process information from input files, simulates process execution, memory
 * allocation, and I/O operations, and logs the state transitions and memory status to output files.
 *
 */
#include "interrupts_101193772_101151910.hpp"

pcbLine processes[MAX_PROCESSES];
int process_count;
int clock;

int ready_queue[MAX_PROCESSES];
int ready_count = 0;

int io_queue[MAX_PARTITIONS];
int io_count = 0;

MemoryPartition partitions[] = {
    {1, 40, -1},
    {2, 25, -1},
    {3, 15, -1},
    {4, 10, -1},
    {5, 8, -1},
    {6, 2, -1}
};

/*
 * stateToString converts a state enum to a string representation 
 */
const char *stateToString(States state)
{
    switch (state)
    {
    case NEW:
        return "NEW";
    case READY:
        return "READY";
    case RUNNING:
        return "RUNNING";
    case WAITING:
        return "WAITING";
    case TERMINATED:
        return "TERMINATED";
    case ERROR:
        return "ERROR";
    }
    return "UNKNOWN";
}

/*
 * logExecution logs a process state transition to the execution log file
 */
void logExecution(FILE *exec_file, int time, int pid, States old_state, States new_state)
{
    fprintf(exec_file, "| %3d | %3d | %-9s | %-10s |\n", time, pid, stateToString(old_state), stateToString(new_state));
}

/* 
 * logMemoryStatus logs the memory status to the memory status file
 */
void logMemoryStatus(FILE *memory_file, int time)
{
    int usable_free = 0;
    int memory_used = 0;

    int partitions_state[MAX_PARTITIONS];
    for (int i = 0; i < MAX_PARTITIONS; i++)
    {
        partitions_state[i] = partitions[i].occupied_by;
        if (partitions[i].occupied_by == -1)
        {
            usable_free += partitions[i].partition_size;
        }
        else
        {
            int process_index = getProcessIndexByPID(partitions[i].occupied_by);
            if (process_index != -1)
            {
                memory_used += processes[process_index].memory_size;
            }
        }
    }

    int total_free = 100 - memory_used;

    fprintf(memory_file, "| %3d | %3d | ", time, memory_used);
    for (int i = 0; i < MAX_PARTITIONS; i++)
    {
        if (i > 0)
            fprintf(memory_file, ", ");
        fprintf(memory_file, "%d", partitions_state[i]);
    }
    fprintf(memory_file, " | %3d | %3d |\n", total_free, usable_free);
}

/* 
 * readFCFSInputFile reads the input file for the First-Come, First-Served (FCFS) scheduler 
 */
void readFCFSInputFile(const char *input_file)
{
    FILE *file = fopen(input_file, "r");
    if (!file)
    {
        perror("Error in opening input file");
        exit(1);
    }

    char buffer[1024];
    process_count = 0;

    int line_number = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        line_number++;
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0)
            continue;

        pcbLine process;
        int values = sscanf(buffer, "%d, %d, %d, %d, %d, %d",
                            &process.pid,
                            &process.memory_size,
                            &process.arrival_time,
                            &process.burst_time,
                            &process.io_frequency,
                            &process.io_duration_tracker);
        process.time_since_last_io = 0;
        process.state = NEW;
        process.original_io_duration = process.io_duration_tracker;

        if (values != 6)
        {
            fprintf(stderr, "Invalid input at line %d: %s\n", line_number, buffer);
            exit(1);
            if (process_count >= MAX_PROCESSES)
            {
                fprintf(stderr, "Reached maximum number of processes (%d)\n", MAX_PROCESSES);
                exit(1);
            }
            exit(1);
        }

        processes[process_count] = process;
        process_count++;
    }
    fclose(file);
}

/* 
 * readEPInputFile reads the input file for the Earliest Priority (EP) scheduler 
 */
void readEPInputFile(const char *input_file)
{
    FILE *file = fopen(input_file, "r");

    if (!file)
    {
        perror("Error in opening input file");
        exit(1);
    }

    char buffer[1024];
    process_count = 0;

    int line_number = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        line_number++;
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0)
            continue;

        pcbLine process;
        int values = sscanf(buffer, "%d, %d, %d, %d, %d, %d, %d",
                            &process.pid,
                            &process.memory_size,
                            &process.arrival_time,
                            &process.burst_time,
                            &process.io_frequency,
                            &process.io_duration_tracker,
                            &process.priority);
        process.time_since_last_io = 0;
        process.state = NEW;
        process.original_io_duration = process.io_duration_tracker;

        if (values != 7)
        {
            fprintf(stderr, "Invalid input at line %d: %s\n", line_number, buffer);
            exit(1);
            if (process_count >= MAX_PROCESSES)
            {
                fprintf(stderr, "Reached maximum number of processes (%d)\n", MAX_PROCESSES);
                exit(1);
            }
            exit(1);
        }

        processes[process_count] = process;
        process_count++;
    }
    fclose(file);
}

/* 
 * readRRInputFile reads the input file for the Round Robin (RR) scheduler 
 */
void readRRInputFile(const char *input_file)
{
    FILE *file = fopen(input_file, "r");

    if (!file)
    {
        perror("Error in opening input file");
        exit(1);
    }

    char buffer[1024];
    process_count = 0;

    int line_number = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        line_number++;
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0)
            continue;

        pcbLine process;
        int values = sscanf(buffer, "%d, %d, %d, %d, %d, %d",
                            &process.pid,
                            &process.memory_size,
                            &process.arrival_time,
                            &process.burst_time,
                            &process.io_frequency,
                            &process.io_duration_tracker);
        process.time_since_last_io = 0;
        process.state = NEW;
        process.original_io_duration = process.io_duration_tracker;
        process.time_in_memory = 0;

        if (values != 6)
        {
            fprintf(stderr, "Invalid input at line %d: %s\n", line_number, buffer);
            exit(1);
            if (process_count >= MAX_PROCESSES)
            {
                fprintf(stderr, "Reached maximum number of processes (%d)\n", MAX_PROCESSES);
                exit(1);
            }
            exit(1);
        }

        processes[process_count] = process;
        process_count++;
    }
    fclose(file);
}

/*
 * getProcessIndexByPID returns the index of a process in the processes array based on its PID
 */
int getProcessIndexByPID(int pid)
{
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].pid == pid)
        {
            return i;
        }
    }
    return -1;
}

/*
 * findHighestPriority returns the index of the process with the highest priority in the ready queue
 */
int findHighestPriority()
{
    int highest_priority = 10; // 0 is highest 9 is lowest
    int earliest_arrival_time = MAX_SIMULATION_TIME + 1;
    int highest_priority_process = -1;

    for (int i = 0; i < ready_count; i++)
    {
        int current = getProcessIndexByPID(ready_queue[i]);
        if (current != -1 && processes[current].state == READY)
        {
            if (processes[current].priority < highest_priority)
            {
                highest_priority = processes[current].priority;
                earliest_arrival_time = processes[current].arrival_time;
                highest_priority_process = current;
            }
            else if (processes[current].priority == highest_priority && processes[current].arrival_time < earliest_arrival_time)
            {
                earliest_arrival_time = processes[current].arrival_time;
                highest_priority_process = current;
            }
        }
    }

    return highest_priority_process;
}

/*
 * runningProcessExists checks if there is a process in the RUNNING state
 */
bool runningProcessExists()
{
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].state == RUNNING)
        {
            return true;
        }
    }
    return false;
}

/*
 * allocateMemory allocates memory for a process based on the best-fit algorithm
 */
int allocateMemory(pcbLine *process, FILE *memory_file)
{
    for (int i = MAX_PARTITIONS - 1; i >= 0; i--)
    {
        if (partitions[i].occupied_by == -1 && partitions[i].partition_size >= process->memory_size)
        {
            partitions[i].occupied_by = process->pid;
            logMemoryStatus(memory_file, clock);
            return partitions[i].partition_id;
        }
    }
    return -1;
}

/*
 * deallocateMemory deallocates memory for a process
 */
void deallocateMemory(pcbLine *process, FILE *memory_file)
{
    for (int i = 0; i < MAX_PARTITIONS; i++)
    {
        if (partitions[i].occupied_by == process->pid)
        {
            partitions[i].occupied_by = -1;
            logMemoryStatus(memory_file, clock);
            return;
        }
    }
}

/*
 * enqueue adds a process to a queue
 */
void enqueue(int queue[], int pid, int *count)
{
    if (*count < MAX_PARTITIONS)
    {
        queue[*count] = pid;
        (*count)++;
    }
    else
    {
        fprintf(stderr, "Queue is full, cannot enqueue PID %d\n", pid);
    }
}

/*
 * dequeue removes a process from a queue
 */
int dequeue(int queue[], int *count)
{
    if (*count == 0)
        return -1;
    int pid = queue[0];
    for (int i = 1; i < *count; i++)
    {
        queue[i - 1] = queue[i];
    }
    (*count)--;
    return pid;
}

/*
 * dequeueSpecific removes a specific process from a queue
 */
void dequeueSpecific(int queue[], int *count, int pid)
{
    int index = -1;
    for (int i = 0; i < *count; i++)
    {
        if (queue[i] == pid)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        for (int i = index; i < (*count) - 1; i++)
        {
            queue[i] = queue[i + 1];
        }
        (*count)--;
    }
    else
    {
        fprintf(stderr, "Error: PID %d not found in queue\n", pid);
    }
}

/*
 * checkProcessArrivals checks if any new processes have arrived and allocates memory for them
 */
void checkProcessArrivals(FILE *exec_file, FILE *memory_file)
{
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].arrival_time <= clock && processes[i].state == NEW)
        {
            int partition_index = allocateMemory(&processes[i], memory_file);
            if (partition_index != -1)
            {
                logExecution(exec_file, clock, processes[i].pid, NEW, READY);
                processes[i].state = READY;
                enqueue(ready_queue, processes[i].pid, &ready_count);
            }
        }
    }
}

/*
 * processFCFS simulates process execution for the First-Come, First-Served (FCFS) scheduler
 */
void processFCFS(FILE *exec_file, FILE *memory_file)
{
    int running_process = -1;
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].state == RUNNING)
        {
            running_process = i;
            break;
        }
    }

    if (running_process == -1 && ready_count > 0)
    {
        int next_process_pid = dequeue(ready_queue, &ready_count);
        if (next_process_pid != -1)
        {
            int next_process_index = getProcessIndexByPID(next_process_pid);
            pcbLine *next_process = &processes[next_process_index];
            logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
            next_process->state = RUNNING;
            running_process = next_process_index;
        }
    }
    if (running_process != -1)
    {
        pcbLine *current_process = &processes[running_process];

        if (current_process->burst_time <= 0)
        {
            logExecution(exec_file, clock, current_process->pid, RUNNING, TERMINATED);
            current_process->state = TERMINATED;
            deallocateMemory(current_process, memory_file);
        }
        else if (current_process->time_since_last_io == current_process->io_frequency)
        {
            current_process->time_since_last_io = 0;
            logExecution(exec_file, clock, current_process->pid, RUNNING, WAITING);
            current_process->state = WAITING;
            enqueue(io_queue, current_process->pid, &io_count);
        }else
        {
            current_process->burst_time--;
            current_process->time_since_last_io++;
        }
    }
}

/*
 * processIO simulates I/O operations for processes in the I/O queue
 */
void processIO(FILE *exec_file)
{
    for (int i = 0; i < io_count; i++)
    {
        int process_index = getProcessIndexByPID(io_queue[0]);
        if (process_index == -1)
        {
            fprintf(stderr, "Process not found\n");
            exit(1);
        }

        pcbLine *current_process = &processes[process_index];

        if (current_process->io_duration_tracker <= 0)
        {
            current_process->io_duration_tracker = current_process->original_io_duration;
            logExecution(exec_file, clock, current_process->pid, WAITING, READY);
            current_process->state = READY;

            enqueue(ready_queue, current_process->pid, &ready_count);
            dequeue(io_queue, &io_count);
            i--;
        }
        else
        {
            current_process->io_duration_tracker--;
        }
    }
}

/*
 * processEP simulates process execution for the Earliest Priority (EP) scheduler
 */
void processEP(FILE *exec_file, FILE *memory_file)
{
    int running_process = -1;

    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].state == RUNNING)
        {
            running_process = i;
            break;
        }
    }

    if (running_process == -1)
    {
        int highest_priority_process = findHighestPriority();
        if (highest_priority_process != -1)
        {
            pcbLine *current_process = &processes[highest_priority_process];
            logExecution(exec_file, clock, current_process->pid, READY, RUNNING);
            current_process->state = RUNNING;
            dequeueSpecific(ready_queue, &ready_count, current_process->pid);
        }
    }
    else
    {
        pcbLine *current_process = &processes[running_process];
        current_process->burst_time--;
        current_process->time_since_last_io++;

        if (current_process->burst_time <= 0)
        {
            logExecution(exec_file, clock, current_process->pid, RUNNING, TERMINATED);
            current_process->state = TERMINATED;
            deallocateMemory(current_process, memory_file);
            running_process = -1;

            int highest_priority_process = findHighestPriority();
            if (highest_priority_process != -1)
            {
                pcbLine *next_process = &processes[highest_priority_process];
                logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
                next_process->state = RUNNING;
                dequeueSpecific(ready_queue, &ready_count, next_process->pid);
            }
        }
        else if (current_process->time_since_last_io == current_process->io_frequency)
        {
            current_process->time_since_last_io = 0;
            logExecution(exec_file, clock, current_process->pid, RUNNING, WAITING);
            current_process->state = WAITING;
            enqueue(io_queue, current_process->pid, &io_count);
            running_process = -1;

            int highest_priority_process = findHighestPriority();
            if (highest_priority_process != -1)
            {
                pcbLine *next_process = &processes[highest_priority_process];
                logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
                next_process->state = RUNNING;
                dequeueSpecific(ready_queue, &ready_count, next_process->pid);
            }
        }
    }
}

/*
 * processRR simulates process execution for the Round Robin (RR) scheduler
 */ 
void processRR(FILE *exec_file, FILE *memory_file)
{
    int running_process = -1;

    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].state == RUNNING)
        {
            running_process = i;
            break;
        }
    }

    if (running_process == -1 && ready_count > 0)
    {
        int next_process_pid = ready_queue[0];
        int next_process_index = getProcessIndexByPID(next_process_pid);

        if (next_process_index != -1)
        {
            pcbLine *next_process = &processes[next_process_index];
            logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
            next_process->state = RUNNING;
            next_process->time_in_memory = 0;
            dequeue(ready_queue, &ready_count);
        }
    }
    else if (running_process != -1)
    {
        pcbLine *current_process = &processes[running_process];
        current_process->burst_time--;
        current_process->time_since_last_io++;
        current_process->time_in_memory++;

        if (current_process->burst_time <= 0)
        {
            logExecution(exec_file, clock, current_process->pid, RUNNING, TERMINATED);
            current_process->state = TERMINATED;
            deallocateMemory(current_process, memory_file);
            running_process = -1;

            if (ready_count > 0)
            {
                int next_process_pid = ready_queue[0];
                int next_process_index = getProcessIndexByPID(next_process_pid);
                if (next_process_index != -1)
                {
                    pcbLine *next_process = &processes[next_process_index];
                    logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
                    next_process->state = RUNNING;
                    next_process->time_in_memory = 0;
                    dequeue(ready_queue, &ready_count);
                }
            }
        }
        else if (current_process->time_in_memory == 100)
        {
            logExecution(exec_file, clock, current_process->pid, RUNNING, READY);
            current_process->state = READY;
            enqueue(ready_queue, current_process->pid, &ready_count);
            running_process = -1;

            if (ready_count > 0)
            {
                int next_process_pid = dequeue(ready_queue, &ready_count);
                int next_process_index = getProcessIndexByPID(next_process_pid);
                if (next_process_index != -1)
                {
                    pcbLine *next_process = &processes[next_process_index];
                    logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
                    next_process->state = RUNNING;
                    next_process->time_in_memory = 0;
                }
            }
        }
        else if (current_process->time_since_last_io == current_process->io_frequency)
        {
            current_process->time_since_last_io = 0;
            logExecution(exec_file, clock, current_process->pid, RUNNING, WAITING);
            current_process->state = WAITING;
            enqueue(io_queue, current_process->pid, &io_count);
            running_process = -1;

            if (ready_count > 0)
            {
                int next_process_pid = ready_queue[0];
                int next_process_index = getProcessIndexByPID(next_process_pid);
                if (next_process_index != -1)
                {
                    pcbLine *next_process = &processes[next_process_index];
                    logExecution(exec_file, clock, next_process->pid, READY, RUNNING);
                    next_process->state = RUNNING;
                    next_process->time_in_memory = 0;
                    dequeue(ready_queue, &ready_count);
                }
            }
        }
    }
}

/*
 * simulatorFCFS simulates the First-Come, First-Served (FCFS) scheduler
 */
void simulatorFCFS(char input_file[], FILE *exec_file, FILE *memory_file)
{
    clock = 0;

    fprintf(exec_file, "+------------------------------------------------+\n");
    fprintf(exec_file, "| Time | PID | Old State | New State |\n");
    fprintf(exec_file, "+------------------------------------------------+\n");

    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
    fprintf(memory_file, "| Time of Event | Memory Used | Partitions State          | Total Free Memory | Usable Free Memory |\n");
    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
    fprintf(memory_file, "|   0 |   0 | -1, -1, -1, -1, -1, -1 | 100 | 100 |\n");

    readFCFSInputFile(input_file);
    checkProcessArrivals(exec_file, memory_file);

    while ((ready_count > 0 || io_count > 0 || runningProcessExists()) && (clock < MAX_SIMULATION_TIME))
    {
        checkProcessArrivals(exec_file, memory_file);
        processFCFS(exec_file, memory_file);
        processIO(exec_file);
        clock++;
    }

    printf("FCFS Simulation completed\n");
    fprintf(exec_file, "+------------------------------------------------+\n");
    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
}

/*
 * simulatorEP simulates the Earliest Priority (EP) scheduler
 */
void simulatorEP(char input_file[], FILE *exec_file, FILE *memory_file)
{

    clock = 0;

    fprintf(exec_file, "+------------------------------------------------+\n");
    fprintf(exec_file, "| Time | PID | Old State | New State |\n");
    fprintf(exec_file, "+------------------------------------------------+\n");

    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
    fprintf(memory_file, "| Time of Event | Memory Used | Partitions State          | Total Free Memory | Usable Free Memory |\n");
    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
    fprintf(memory_file, "|   0 |   0 | -1, -1, -1, -1, -1, -1 | 100 | 100 |\n");

    readEPInputFile(input_file);

    checkProcessArrivals(exec_file, memory_file);

    while ((ready_count > 0 || io_count > 0 || runningProcessExists()) && (clock < MAX_SIMULATION_TIME))
    {
        checkProcessArrivals(exec_file, memory_file);
        processEP(exec_file, memory_file);
        processIO(exec_file);
        clock++;
    }

    printf("EP Simulation completed\n");
    fprintf(exec_file, "+------------------------------------------------+\n");
    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
}

/*
 * simulatorRR simulates the Round Robin (RR) scheduler
 */
void simulatorRR(char input_file[], FILE *exec_file, FILE *memory_file)
{
    clock = 0;

    fprintf(exec_file, "+------------------------------------------------+\n");
    fprintf(exec_file, "| Time | PID | Old State | New State |\n");
    fprintf(exec_file, "+------------------------------------------------+\n");

    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
    fprintf(memory_file, "| Time of Event | Memory Used | Partitions State          | Total Free Memory | Usable Free Memory |\n");
    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
    fprintf(memory_file, "|   0 |   0 | -1, -1, -1, -1, -1, -1 | 100 | 100 |\n");

    readFCFSInputFile(input_file); // You can reuse the FCFS input format

    checkProcessArrivals(exec_file, memory_file);

    while ((ready_count > 0 || io_count > 0 || runningProcessExists()) && (clock < MAX_SIMULATION_TIME))
    {
        checkProcessArrivals(exec_file, memory_file);
        processRR(exec_file, memory_file);
        processIO(exec_file);
        clock++;
    }

    printf("RR Simulation completed\n");
    fprintf(exec_file, "+------------------------------------------------+\n");
    fprintf(memory_file, "+------------------------------------------------------------------------------------------+\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Argument Error\n");
        exit(1);
    }

    FILE *exec_file = fopen("execution_101193772_101151910.txt", "w");
    if (!exec_file)
    {
        perror("Unable to open log file");
        return EXIT_FAILURE;
    }

    FILE *memory_file = fopen("memory_status_101193772_101151910.txt", "w");
    if (!memory_file)
    {
        perror("Unable to open memory status file");
        fclose(exec_file);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "FCFS") == 0)
    {
        simulatorFCFS(argv[1], exec_file, memory_file);
    }
    else if (strcmp(argv[2], "EP") == 0)
    {
        simulatorEP(argv[1], exec_file, memory_file);
    }
    else if (strcmp(argv[2], "RR") == 0)
    {
        simulatorRR(argv[1], exec_file, memory_file);
    }
    fclose(exec_file);
    fclose(memory_file);
    return 0;
}
