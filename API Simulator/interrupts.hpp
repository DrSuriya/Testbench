/*
 * Author: Hasan Suriya - 101193772
 * Co-Author: Muhammas Maisam- 101151910
 * Course: SYSC 4001 - Operating Systems
 * Assignment: Assignment 2 
 * 
 * Date: 1-11-2024 
 * 
 * Description:
 * Contains the structure, constants, function prototypes for the functions in interrupts.cpp.
 * 
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <iostream>


// Functions from Assignment 1
#define MAX_VECTOR_SIZE 50
#define MAX_TRACE_SIZE 1000

typedef struct {
    char type[10]; // Holds intructions such as CPU, SYSCALL, or END_IO
    int interrupt; // Holds the interrupt number for SYSCALL and END_IO events
    int duration; // Holds the duration of the events
} programLine; // changed from traceline to reflect the current usage more accurately

extern unsigned int vectorTable[]; //Holds the ISR addresses that are read form the vector table file
extern int vectorEntries; //Holds the number of addresses in the vector table
extern programLine traceEvents[]; //Holds the events that are read from the trace file
extern int countTraceLines; //Holds the number of events in the trace file

void readVectorTable(const char* filename); //Reads a vector table file and stores the ISR addresses in the vectorTable list
//void readInputFile(const char* filename); not used in assignment 2 
void handleCPUEvent(programLine* event, int* clockCPU, FILE *outputFile); //Handles CPU events
void handleSysCallEvent(programLine* event, int* clockCPU, FILE *outputFile); //Handles SYSCALL events
void handleIOEvent(programLine* event, int* clockCPU, FILE *outputFile); //Handles END_IO events
void executable(char* filename); //Executes the simulation based on the events in the trace file

// Functions for Assignment 2
extern int cpuClock;
typedef struct {          
    char program_name[20];  
    unsigned int memory_needed;           
} externalFile;

typedef struct {
    char type[10];          
    char program_name[20];  
    int duration;           
} traceLine;

typedef struct PCB {
    int pid;
    int cpu_time_used;
    int io_time_remaining;    
    int cpu_time_remaining;   
    int partition_number;
} PCB;

typedef struct MemoryPartition {
    int partition_number;    
    int partition_size;
    char status[20];
} MemoryPartition;

extern externalFile externalfileTable[];
extern int externalFileCount; 
extern traceLine traceEvent[];
extern int countTraceLines; 
extern PCB processes[];
extern int processCounter;

PCB createChildProcess(const PCB* parentProcess, int duration);
void readTraceFile(const char* inputFile);
void readExternalFile(const char* filename);
void handleFork(traceLine* event, int* cpuClock, FILE* outputFile);
void handleExec(traceLine* event, int* cpuClock, FILE* outputFile);
void initializeSimulator();
void simulator(const char* filename); 
void logSystemStatus(int saveTime);


#endif 
