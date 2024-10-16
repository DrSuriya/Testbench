/*
 * Author: Hasan Suriya - 101193772
 * Co-Author: Anique Ali - 101202512
 * Course: SYSC 4001 - Operating Systems
 * Assignment: Assignment 1 - Design and Implementation of an Interrupt Simulator
 * Date: 4-10-2024
 *
 * Description:
 * Contains a interrupt simulator that reads a vector table and a trace file.
 * The simulator then simulates the execution of the events (CPU, SYSCALL, and END_IO) provided in the trace files, 
 * then the an output file is created that logs the CPU's actions. 
 * 
 */

#include "interrupts.hpp"

unsigned int vectorTable[MAX_VECTOR_SIZE];  
int vectorEntries = 0;
traceLine traceEvents[MAX_TRACE_SIZE];  
int countTraceLines = 0;

/* readVectorTable stores the ISR addresses from a Vector Table file into the vectorTable array.
 * This enables the simulator to access the the ISR, through interrupt number provided in the trace files.  
 */
void readVectorTable(const char* vectorFile) { 
    FILE* file = fopen(vectorFile, "r");
    if (!file) {
        perror("Error opening vector table file");
        exit(1);
    }
    char buffer[100];
    vectorEntries = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        unsigned int isrAddress = (unsigned int)strtoul(buffer, NULL, 16);
        vectorTable[vectorEntries++] = isrAddress;
    }
    fclose(file);
}

// readInputFile stores the events provided in a input trace file into the traceEvents Array. 
void readInputFile(const char* inputFIle) {
    FILE* file = fopen(inputFIle, "r");
    if (!file) {
        perror("Error opening trace file");
        exit(1);
    }
    char buffer[100];
    countTraceLines = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0)
            continue;
        traceLine event;
        if (strncmp(buffer, "CPU,", 4) == 0) {
            sscanf(buffer, "CPU, %d", &event.duration);
            strcpy(event.type, "CPU");
            event.interrupt = -1; 
        } else if (strncmp(buffer, "SYSCALL", 7) == 0) {
            sscanf(buffer, "SYSCALL %d, %d", &event.interrupt, &event.duration);
            strcpy(event.type, "SYSCALL");
        } else if (strncmp(buffer, "END_IO", 6) == 0) {
            sscanf(buffer, "END_IO %d, %d", &event.interrupt, &event.duration);
            strcpy(event.type, "END_IO");
        } else {
            fprintf(stderr, "Unknown line in trace file: %s\n", buffer);
            exit(1);
        }
        traceEvents[countTraceLines++] = event;
    }
    fclose(file);
}

// handleCPUEvent simulates the CPU event by logging the CPU execution and updating the cpuClock.
void handleCPUEvent(traceLine* event, int* cpuClock, FILE *outputFile) {
    fprintf(outputFile, "%d, %d, CPU execution\n", *cpuClock, event->duration);
    *cpuClock += event->duration;
}

/* handleSysCallEvent simulates the SYSCALL event by logging the switching to kernel mode, saving context,
 * finding the ISR in the vector table, loading the ISR, running the ISR, transferring data,
 * checking for errors, and returning from the event.
 */
void handleSysCallEvent(traceLine* event, int* cpuClock, FILE *outputFile) {
    int kernalSwitch = 1;
    fprintf(outputFile, "%d, %d, switch to kernel mode\n", *cpuClock, kernalSwitch);
    *cpuClock += kernalSwitch;

    int contextSaveTime = rand() % 3 + 1; 
    fprintf(outputFile, "%d, %d, context saved\n", *cpuClock, contextSaveTime);
    *cpuClock += contextSaveTime;

    int findVector = 1;
    fprintf(outputFile, "%d, %d, find vector %d in memory position 0x%02X\n", *cpuClock, findVector, event->interrupt, event->interrupt * 2);
    *cpuClock += findVector;

    unsigned int address = vectorTable[event->interrupt];
    int loadAddressTime = 1;
    fprintf(outputFile, "%d, %d, load address 0X%04X into the PC\n", *cpuClock, loadAddressTime, address);
    *cpuClock += loadAddressTime;

    int totalDuration = event->duration;
    int durationISR = totalDuration * 0.4;
    fprintf(outputFile, "%d, %d, SYSCALL: run the ISR\n", *cpuClock, durationISR);
    *cpuClock += durationISR;

    int remainingTime = totalDuration * 0.35;
    int durationTransfer = remainingTime * 0.7; 
    fprintf(outputFile, "%d, %d, transfer data\n", *cpuClock, durationTransfer);
    *cpuClock += durationTransfer;

    int durationErrorCheck = remainingTime * 0.25; 
    fprintf(outputFile, "%d, %d, check for errors\n", *cpuClock, durationErrorCheck);
    *cpuClock += durationErrorCheck;

    int timeToReturn = 1;
    fprintf(outputFile, "%d, %d, IRET\n", *cpuClock, timeToReturn);
    *cpuClock += timeToReturn;
}

/* handleIOEvent simulates the END_IO event by logging checking for interrupt priority, checking if it's
 * masked, switching to kernel mode, saving context, finding the ISR vector, loading the ISR, completing
 * the I/O operation, and returning from the event.
 */
void handleIOEvent(traceLine* event, int* cpuClock, FILE *outputFile) {
    int timetoCheckPriority = 1;
    fprintf(outputFile, "%d, %d, check priority of interrupt\n", *cpuClock, timetoCheckPriority);
    *cpuClock += timetoCheckPriority;

    int checkMasked = 1;
    fprintf(outputFile, "%d, %d, check if masked\n", *cpuClock, checkMasked);
    *cpuClock += checkMasked;
    
    int kernalSwitch = 1;
    fprintf(outputFile, "%d, %d, switch to kernel mode\n", *cpuClock, kernalSwitch);
    *cpuClock += kernalSwitch;

    int contextSaveTime = 3;
    fprintf(outputFile, "%d, %d, context saved\n", *cpuClock, contextSaveTime);
    *cpuClock += contextSaveTime;

    int findVector = 1;
    fprintf(outputFile, "%d, %d, find vector %d in memory position 0x%04X\n", *cpuClock, findVector, event->interrupt, event->interrupt * 4);
    *cpuClock += findVector;

    unsigned int address = vectorTable[event->interrupt];
    int loadAddress = 1;
    fprintf(outputFile, "%d, %d, load address 0X%04X into the PC\n", *cpuClock, loadAddress, address);
    *cpuClock += loadAddress;

    int totalDuration = event->duration;
    fprintf(outputFile, "%d, %d, END_IO\n", *cpuClock, totalDuration);
    *cpuClock += totalDuration;

    int durationReturn = 1;
    fprintf(outputFile, "%d, %d, IRET\n", *cpuClock, durationReturn);
    *cpuClock += durationReturn;
}

/* 
 * executable simulates the execution of all events from the trace file.
 */
void executable(char* filename) {
    int cpuClock = 0;
    
    FILE *outputFile = fopen(filename, "w");
    if (!outputFile) {
        perror("Error opening output file");
        exit(1);
    }
    
    for (int i = 0; i < countTraceLines; i++) {
        traceLine* event = &traceEvents[i]; 
        char eventType[10];
        strcpy(eventType, event->type);
        if (strcmp(eventType, "CPU") == 0) {
            handleCPUEvent(event, &cpuClock, outputFile);
        } else if (strcmp(eventType, "SYSCALL") == 0) {
            handleSysCallEvent(event, &cpuClock, outputFile);
        } else if (strcmp(eventType, "END_IO") == 0) {
            handleIOEvent(event, &cpuClock, outputFile);
        } else {
            fprintf(outputFile, "Invalid event: %s\n", eventType);
            exit(1);
        }
    }
    fclose(outputFile);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {  //argc is four because only four arguments should be given the program name, trace file, output file, and vector
        exit(1);
    }
    
    readVectorTable(argv[3]); 
    readInputFile(argv[1]);
    executable(argv[2]);   
    
    return 0;
}
