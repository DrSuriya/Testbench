/**
 * Author: Hasan Suriya - 101193772
 * Co-Author: Muhammas Maisam - 101151910
 * Course: SYSC 4001 - Operating Systems
 * Assignment: Assignment 2 
 * 
 * Date: 1-11-2024 
 * 
 * The simulator handles different types of events such as CPU execution, SYSCALL, and END_IO. 
 * It also supports process creation (FORK) and execution (EXEC) by reading external files and allocating memory partitions.
 * The simulator initializes with a vector table, external files, and trace files, and then processes each event in the trace file.
 * The output is logged to a specified output file.
 */

#include "interrupts.hpp"
int cpuClock = 0;

unsigned int vectorTable[MAX_VECTOR_SIZE];  
int vectorEntries = 0;
externalFile externalfileTable[MAX_VECTOR_SIZE];  
int externalFileCount= 0;
programLine programEvents[MAX_TRACE_SIZE];  
int countprogramLines = 0;
traceLine traceEvent[MAX_TRACE_SIZE];
int countTraceLines = 0;
PCB processes[MAX_VECTOR_SIZE];  
int processCounter = 0;

/* 
 * readVectorTable stores the ISR addresses from a Vector Table file into the vectorTable array.
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

// readProgramFile stores the events provided in a input trace file into the programEvents Array. 
void readProgramFile(const char* inputFIle) {
    FILE* file = fopen(inputFIle, "r");
    if (!file) {
        perror("Error opening trace file");
        exit(1);
    }
    char buffer[100];
    countprogramLines = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0)
            continue;
        programLine event;
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
        programEvents[countprogramLines++] = event;
    }
    fclose(file);
}

// handleCPUEvent simulates the CPU event by logging the CPU execution and updating the cpuClock.
void handleCPUEvent(programLine* event, int* cpuClock, FILE *outputFile) {
    fprintf(outputFile, "%d, %d, CPU execution\n", *cpuClock, event->duration);
    *cpuClock += event->duration;
}

/* 
 * handleSysCallEvent simulates the SYSCALL event by logging the switching to kernel mode, saving context,
 * finding the ISR in the vector table, loading the ISR, running the ISR, transferring data,
 * checking for errors, and returning from the event.
 */
void handleSysCallEvent(programLine* event, int* cpuClock, FILE *outputFile) {
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

/*
 * handleIOEvent simulates the END_IO event by logging checking for interrupt priority, checking if it's
 * masked, switching to kernel mode, saving context, finding the ISR vector, loading the ISR, completing
 * the I/O operation, and returning from the event.
 */
void handleIOEvent(programLine* event, int* cpuClock, FILE *outputFile) {
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
void executable(const char* program_name, FILE* outputFile) {
    char fileName[50];
    snprintf(fileName, sizeof(fileName), "%s.txt", program_name);  // Add .txt to program_name

    FILE* programFile = fopen(fileName, "r");
    if (!programFile) {
        fprintf(stderr, "Error opening program file: %s\n", fileName);
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), programFile)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0) continue;

        programLine event;
        traceLine traceEvent;
        if (strncmp(buffer, "CPU,", 4) == 0) {
            sscanf(buffer, "CPU, %d", &event.duration);
            strcpy(event.type, "CPU");
            handleCPUEvent(&event, &cpuClock, outputFile);
        } else if (strncmp(buffer, "SYSCALL", 7) == 0) {
            sscanf(buffer, "SYSCALL %d, %d", &event.interrupt, &event.duration);
            strcpy(event.type, "SYSCALL");
            handleSysCallEvent(&event, &cpuClock, outputFile);
        } else if (strncmp(buffer, "END_IO", 6) == 0) {
            sscanf(buffer, "END_IO %d, %d", &event.interrupt, &event.duration);
            strcpy(event.type, "END_IO");
            handleIOEvent(&event, &cpuClock, outputFile);
        } else if (strncmp(buffer, "FORK,", 5) == 0) {
            sscanf(buffer, "FORK, %d", &traceEvent.duration);
            strcpy(traceEvent.type, "FORK");
            handleFork(&traceEvent, &cpuClock, outputFile);
        } else if (strncmp(buffer, "EXEC", 4) == 0) {
            sscanf(buffer, "EXEC %[^,], %d", traceEvent.program_name, &traceEvent.duration);
            strcpy(traceEvent.type, "EXEC");
            handleExec(&traceEvent, &cpuClock, outputFile);
        }else {
            fprintf(stderr, "Unknown line in program file: %s\n", buffer);
            fclose(programFile);
            exit(1);
        }
    }

    fclose(programFile);
}

MemoryPartition partitions[] = {
    {1, 40, "free"},
    {2, 25, "free"},
    {3, 15, "free"},
    {4, 10, "free"},
    {5, 8, "free"},
    {6, 2, "free"}
};

PCB createChildProcess(const PCB* parentProcess, int duration) {
    PCB childPCB;
    childPCB.pid = processCounter++;
    childPCB.cpu_time_used = 0;
    childPCB.io_time_remaining = parentProcess->io_time_remaining;
    childPCB.cpu_time_remaining = duration;
    childPCB.partition_number = parentProcess->partition_number;

    return childPCB;
}

void readTraceFile(const char* inputFile) {
    FILE* file = fopen(inputFile, "r");
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
        if (strncmp(buffer, "FORK,", 5) == 0) {
            strcpy(event.type, "FORK");
            sscanf(buffer, "FORK, %d", &event.duration);
            strcpy(event.program_name, "");
        } 
        else if (strncmp(buffer, "EXEC ", 5) == 0) {
            strcpy(event.type, "EXEC");
            sscanf(buffer, "EXEC %[^,], %d", event.program_name, &event.duration);
        } 
        else {
            fprintf(stderr, "Unknown line in trace file: %s\n", buffer);
            exit(1);
        }
        
        traceEvent[countTraceLines++] = event;
    }
    fclose(file);
}

void readExternalFile(const char* filename) { 
    FILE* input = fopen(filename, "r");
    if (!input) {
        perror("Error opening external file");
        exit(1);
    }

    char buffer[100];
    externalFileCount = 0;

    while (fgets(buffer, sizeof(buffer), input)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (sscanf(buffer, "%[^,], %u", externalfileTable[externalFileCount].program_name, &externalfileTable[externalFileCount].memory_needed) == 2) {
            externalFileCount++;
        } else {
            fprintf(stderr, "Error parsing line in external file: %s\n", buffer);
        }
    }
    
    fclose(input);
}

void handleFork(traceLine* event, int* cpuClock, FILE* outputFile) {

    int kernelSwitchTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, switch to kernel mode\n", *cpuClock, kernelSwitchTime);
    *cpuClock += kernelSwitchTime;

    int contextSaveTime = rand() % 10 + 1; 
    fprintf(outputFile, "%d, %d, context saved\n", *cpuClock, contextSaveTime);
    *cpuClock += contextSaveTime;

    int forkVectorIndex = 2; // 2 cant be used elsewhere now
    unsigned int address = vectorTable[forkVectorIndex];

    int loadAddressTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, load address 0X%04X into the PC\n", *cpuClock, loadAddressTime, address);
    *cpuClock += loadAddressTime;

    int durationISR = event->duration * 0.4;
    fprintf(outputFile, "%d, %d, FORK: copying parent PCB to child PCB\n", *cpuClock, durationISR);
    *cpuClock += durationISR;

    PCB childPCB = createChildProcess(&processes[processCounter - 1], event->duration);
    processes[childPCB.pid] = childPCB;

    int schedulerCallTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, scheduler called\n", *cpuClock, schedulerCallTime);
    *cpuClock += schedulerCallTime;

    int returnISRTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, IRET\n", *cpuClock, returnISRTime);
    *cpuClock += returnISRTime;

    logSystemStatus(*cpuClock);
}

void handleExec(traceLine* event, int* cpuClock, FILE* outputFile) {
    int kernelSwitchTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, switch to kernel mode\n", *cpuClock, kernelSwitchTime);
    *cpuClock += kernelSwitchTime;

    int contextSaveTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, context saved\n", *cpuClock, contextSaveTime);
    *cpuClock += contextSaveTime;

    int execVectorIndex = 1; // 1 cant be used elsewhere now
    unsigned int address = vectorTable[execVectorIndex];
    
    int loadAddressTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, load address 0X%04X into the PC\n", *cpuClock, loadAddressTime, address);
    *cpuClock += loadAddressTime;

    int durationISR = event->duration * 0.4;
    fprintf(outputFile, "%d, %d, EXEC: loading %s\n", *cpuClock, durationISR, event->program_name);
    *cpuClock += durationISR;

    int programSize = -1;
    for (int i = 0; i < externalFileCount; i++) {
        if (strcmp(externalfileTable[i].program_name, event->program_name) == 0) {
            programSize = externalfileTable[i].memory_needed;
            break;
        }
    }

    if (programSize == -1) {
        fprintf(stderr, "Error: Program %s not found in external files.\n", event->program_name);
        exit(1);
    }

    int bestFitIndex = -1;
    int minSizeDifference = __INT_MAX__;
    for (int i = 0; i < sizeof(partitions) / sizeof(partitions[0]); i++) {
        if (strcmp(partitions[i].status, "free") == 0 && partitions[i].partition_size >= programSize) {
            int sizeDifference = partitions[i].partition_size - programSize;
            if (sizeDifference < minSizeDifference) {
                minSizeDifference = sizeDifference;
                bestFitIndex = i;
            }
        }
    }

    if (bestFitIndex == -1) {
        fprintf(stderr, "Error: No suitable partition found for program %s\n", event->program_name);
        exit(1);
    }

    strcpy(partitions[bestFitIndex].status, event->program_name);
    fprintf(outputFile, "%d, %d, found partition %d with %dMb of space\n", *cpuClock, programSize, partitions[bestFitIndex].partition_number, partitions[bestFitIndex].partition_size);
    *cpuClock += rand() % 10 + 1;

    fprintf(outputFile, "%d, %d, partition %d marked as occupied\n", *cpuClock, 6, partitions[bestFitIndex].partition_number);
    *cpuClock += rand() % 10 + 1;

    PCB *currentProcess = &processes[processCounter - 1];
    currentProcess->partition_number = partitions[bestFitIndex].partition_number;
    currentProcess->cpu_time_remaining = event->duration;
    fprintf(outputFile, "%d, %d, updating PCB with new information\n", *cpuClock, 17);
    *cpuClock += rand() % 10 + 1;

    int schedulerCallTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, scheduler called\n", *cpuClock, schedulerCallTime);
    *cpuClock += schedulerCallTime;

    int returnISRTime = rand() % 10 + 1;
    fprintf(outputFile, "%d, %d, IRET\n", *cpuClock, returnISRTime);
    *cpuClock += returnISRTime;

    executable(event->program_name, outputFile);

    logSystemStatus(*cpuClock);
}

void initializeSimulator() {
    PCB initProcess;
    initProcess.pid = processCounter++;
    initProcess.cpu_time_used = 0;
    initProcess.io_time_remaining = 0;
    initProcess.cpu_time_remaining = 0;
    initProcess.partition_number = 6;

    processes[initProcess.pid] = initProcess;
    strcpy(partitions[5].status, "init");
}

void simulator(const char* filename) {
    cpuClock = 0;
    
    FILE *outputFile = fopen(filename, "w");
    if (!outputFile) {
        perror("Error opening output file");
        exit(1);
    }
    
    for (int i = 0; i < countTraceLines; i++) {
        traceLine* event = &traceEvent[i];

        char eventType[10];
        strcpy(eventType, event->type);
        if (strcmp(eventType, "FORK") == 0) {
            handleFork(event, &cpuClock, outputFile);
        } else if (strcmp(eventType, "EXEC") == 0) {
            handleExec(event, &cpuClock, outputFile);
        } else {
            fprintf(outputFile, "Invalid event: %s\n", eventType);
            exit(1);
        }
    }
    fclose(outputFile);
}

/*
 *  
 */
void logSystemStatus(int saveTime) {
    std::ofstream statusFile("system_status.txt", std::ios::app);

    if (!statusFile) {
        std::cerr << "Error opening system_status.txt for writing\n";
        return;
    }

    statusFile << "!-----------------------------------------------------------!\n";
    statusFile << "Save Time: " << saveTime << " ms\n";
    statusFile <<  "+-----------------------------------------------------------+\n";
    statusFile << "| PID | Program Name | Partition | Size |\n";
    statusFile <<  "+-----------------------------------------------------------+\n";

    for (int i = 0; i < processCounter; i++) {
        std::string programName = (processes[i].partition_number == 6) ? "init" : partitions[processes[i].partition_number - 1].status;

        statusFile << "| " << processes[i].pid
                   << " | " << programName
                   << " | " << processes[i].partition_number
                   << " | " << partitions[processes[i].partition_number - 1].partition_size
                   << " |\n";
    }

    statusFile << "+-----------------------------------------------------------+\n";
    statusFile << "!-----------------------------------------------------------!\n\n";

    statusFile.close();
}

int main(int argc, char* argv[]){
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <trace_file> <external_files_file> <vector_table_file> <output_file>" << std::endl;
        return 1;
    }
    
    initializeSimulator();
    logSystemStatus(cpuClock);

    readTraceFile(argv[1]);
    readExternalFile(argv[2]);
    readVectorTable(argv[3]);

    simulator(argv[4]);
    
    return 0;
}
