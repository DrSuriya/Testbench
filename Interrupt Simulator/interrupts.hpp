/*
 * Author: Hasan Suriya - 101193772
 * Co-Author: Anique Ali - 101202512
 * Course: SYSC 4001 - Operating Systems
 * Assignment: Assignment 1 - Design and Implementation of an Interrupt Simulator
 * Date: 4-Oct-2024
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

#define MAX_VECTOR_SIZE 25
#define MAX_TRACE_SIZE 1000

typedef struct {
    char type[10]; // Holds intructions such as CPU, SYSCALL, or END_IO
    int interrupt; // Holds the interrupt number for SYSCALL and END_IO events
    int duration; // Holds the duration of the events
} traceLine;

extern unsigned int vectorTable[]; //Holds the ISR addresses that are read form the vector table file
extern int vectorEntries; //Holds the number of addresses in the vector table
extern traceLine traceEvents[]; //Holds the events that are read from the trace file
extern int countTraceLines; //Holds the number of events in the trace file

void readVectorTable(const char* filename); //Reads a vector table file and stores the ISR addresses in the vectorTable list
void readInputFile(const char* filename); //Reads a trace file and stores the events in the traceEvents list
void handleCPUEvent(traceLine* event, int* clockCPU, FILE *outputFile); //Handles CPU events
void handleSysCallEvent(traceLine* event, int* clockCPU, FILE *outputFile); //Handles SYSCALL events
void handleIOEvent(traceLine* event, int* clockCPU, FILE *outputFile); //Handles END_IO events
void executable(char* filename); //Executes the simulation based on the events in the trace file

#endif 
