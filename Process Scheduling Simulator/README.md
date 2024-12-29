
# Process Scheduling Simulator
_______________________________________
Introduction  
This project implements three scheduling algorithms:  
- First-Come-First-Serve (FCFS)  
- External Priorities (EP)  
- Round-Robin (RR)  

The simulator executes processes based on the chosen scheduling algorithm, manages memory allocation and deallocation, and logs the execution and memory usage details.
_______________________________________
Files Included  

1. Source Code:  
   - `interrupts_101193772_101151910.cpp` – Contains the implementation of the scheduling algorithms.  

2. Header File:  
   - `interrupts_101193772_101151910.hpp` – Declares constants, structures, and functions used in the system.  

3. Test Files:  
   - `input_data_n.txt` – Input files containing process parameters for test cases.  
   - `test_n.sh` (submitted as a txt file) – Shell scripts to run specific test cases.  

4. Output Files:  
   - `execution_101193772_101151910.txt` – Logs the state transitions of processes during execution.  
   - `memory_status_101193772_101151910.txt` – Records memory utilization and partition states.  
_______________________________________
How to Use  

1. Compile the Code:  
   Run the following command to compile the source code:  
   ```bash
   gcc interrupts_101193772_101151910.cpp -o scheduler
   ```

2. Set Up Test Cases:  
   Convert the `test_n.sh` files to executable scripts:  
   ```bash
   chmod +x test_n.sh
   ```

3. Execute a Test Case:  
   Run a test case using either `bash` or `./`:  
   ```bash
   bash test_n.sh
   ```
   OR  
   ```bash
   ./test_n.sh
   ```

4. View the Results:  
   After execution, the following files will be generated:  
   - `log_101193772_101151910.txt` – Contains process execution logs.  
   - `memory_status_101193772_101151910.txt` – Details memory allocation and utilization.  
_______________________________________
Input File Format  

Each line in the input file represents a process with the following parameters:  
```
<Process ID>, <Memory Size>, <Arrival Time>, <Burst Time>, <I/O Frequency>, <I/O Duration>, <Priority (Required for EP)>
```
_______________________________________
Test Scripts  

Each `test_n.sh` file runs the scheduler with specific input and algorithm selection.

For example `test_1.sh`: 
```
bash
gcc interrupts_101193772_101151910.cpp -o scheduler
./scheduler input_data_1.txt FCFS
```

Replace `FCFS` with `EP` or `RR` for to simulate the other scheduling algorithms.  
_______________________________________
Output Files  

1. `execution_101193772_101151910.txt`: 
    Logs the execution details of processes, including start and end times.

2. `memory_status_101193772_101151910.txt`:  
    Records memory allocation, deallocation, and overall utilization.

