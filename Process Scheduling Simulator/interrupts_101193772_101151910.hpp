/*
 * Author: Hasan Suriya - 101193772
 * Co-Author: Muhammad Maisam - 101151910
 * Course: SYSC 4001 - Operating Systems
 * Assignment: Assignment 1 - Design and Implementation of an Interrupt Simulator
 * Date: 6-12-2024
 *
 * Description:
 * This file contains the definitions and declarations for simulating various
 * process scheduling algorithms and memory management techniques. It includes
 * data structures for representing processes and memory partitions, as well as
 * function prototypes for handling process state transitions, memory allocation,
 * and different scheduling algorithms.
 * 
 */

#ifndef INTERRUPTS_101193772_101151910_HPP
#define INTERRUPTS_101193772_101151910_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIMULATION_TIME 1024 // Arbitrarily chosen value to prevent infinite loops
#define MAX_PROCESSES 10         // Chosen based on number of priority levels
#define MAX_PARTITIONS 6         // Chosen based on the number of memory partitions

// Defined based on Assignment Description
typedef enum
{
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED,
    ERROR
} States;

// Defined based on Assignment Description and the Schedulers Execution
typedef struct
{
    int pid;
    int memory_size;
    int arrival_time;
    int burst_time;
    int io_frequency;
    int io_duration_tracker;
    int original_io_duration;
    int time_since_last_io;
    int time_in_memory;
    int priority;
    States state;
} pcbLine;

// Defined based on Assignment Description
typedef struct MemoryPartition
{
    int partition_id;
    int partition_size;
    int occupied_by;
} MemoryPartition;

extern pcbLine processes[MAX_PROCESSES]; // Array of processes in the system
extern int process_count;                // Number of processes in the processes array
extern int clock;                        // Global clock

extern int ready_queue[MAX_PROCESSES]; // Tracks which processes are in the ready queue
extern int ready_count;                // Count how many processes are in the ready queue

extern int io_queue[MAX_PARTITIONS]; // Tracks which processes are in the IO queue
extern int io_count;                 // Count how many processes are in the IO queue

extern MemoryPartition partitions[MAX_PARTITIONS];

const char *stateToString(States state);                                                   // Used to enable using enum States in logExecution function
void logExecution(FILE *exec_file, int time, int pid, States old_state, States new_state); // Used to log the state transitions of processes
void logMemoryStatus(FILE *mem_file, int time);                                            // Used to log any updates in memory

void readFCFSInputFile(const char *input_file); // Added for First Come First Serve
void readEPInputFile(const char *input_file);   // Added for External Priority
void readRRInputFile(const char *input_file);   // Added for Round Robin

int getProcessIndexByPID(int pid);                       // Returns the index of a process in the processes array based on its PID
int findHighestPriority();                               // Returns the index of the process with the highest priority in the ready queue
int allocateMemory(pcbLine *process, FILE *mem_file);    // Allocates memory for a process
void deallocateMemory(pcbLine *process, FILE *mem_file); // Deallocates memory for a process

void enqueue(int queue[], int pid, int *count);         // Adds a process to a queue
int dequeue(int queue[], int *count);                   // Removes a process from a queue
void dequeueSpecific(int queue[], int *count, int pid); // Removes a specific process from a queue

void checkProcessArrivals(FILE *exec_file, FILE *mem_file); // Checks if any processes have arrived and allocates memory for them
void processFCFS(FILE *exec_file, FILE *mem_file);          // Processes the FCFS scheduler
void processIO(FILE *exec_file);                            // Processes the IO
void processEP(FILE *exec_file, FILE *memory_file);         // Processes the External Priority scheduler
void processRR(FILE *exec_file, FILE *mem_file);            // Processes the Round Robin scheduler

void simulatorFCFS(char inputFile[], FILE *exec_file, FILE *mem_file); // Simulates the First Come First Serve scheduler
void simulatorEP(char inputFile[], FILE *exec_file, FILE *mem_file);   // Simulates the External Priority scheduler
void simulatorRR(char inputFile[], FILE *exec_file, FILE *mem_file);   // Simulates the Round Robin scheduler

#endif
