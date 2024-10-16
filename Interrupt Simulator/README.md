# Interrupt Simulator Project

## Overview

This project simulates an interrupt handling mechanism for a system based on input trace files and a vector table. The program processes system calls (SYSCALL) and I/O events (END_IO) by mapping them to interrupt service routines (ISRs) using a vector table.

The project consists of the following key files:

- **interrupts.hpp**: Contains the header definitions for interrupt handling, including function prototypes and necessary data structures.
- **interrupts.cpp**: Implements the main functionality for the interrupt simulation, processing system calls and I/O events from input trace files.
- **vector_table.txt**: Contains hexadecimal addresses used to map interrupts to their corresponding ISR. Each address represents an entry in the vector table.
- **input.txt**: Contains a sequence of CPU instructions, SYSCALLs, and END_IO events that are processed by the interrupt simulator.
- **input.sh**: Shell script used to compile and run the interrupt simulator. It takes `input.txt`, `output.txt`, and `vector_table.txt` as input files.
- **output.txt**: Contains the detailed output of the simulation, showing how each interrupt was handled, including context switches, the transfer of control to the ISR, and the data transfer steps.

## How to Run the Project

1. Ensure you have the necessary permissions to execute the shell script:
   ```bash
   chmod +x input.sh
   ```

2. Run the shell script to compile the program and execute the interrupt simulation:
   ```bash
   ./input.sh
   ```

   The script compiles `interrupts.cpp` and runs the resulting executable with the required input files (`input.txt`, `output.txt`, and `vector_table.txt`).

## Input Files

- **input.txt**: This file contains the trace of CPU instructions, SYSCALLs, and END_IO events processed by the simulator.
  
- **vector_table.txt**: This file holds the vector table used to map interrupts to ISR addresses. Each line represents a hexadecimal ISR address used to retrieve the corresponding interrupt service routine (ISR).

- **input.sh**: This file contains the bash commands to compile the simulator and execute it with the provided input and vector table.

## Output

The results of the simulation are written to `output.txt`, detailing how each interrupt was handled, context switches, and the execution of interrupt service routines (ISRs).
