# Readme

**Roll Number:** MT25xxx  
**Course:** Operating Systems / Systems Programming  
**Project:** Process vs. Thread Performance and Scalability Analysis

## 1. Description

This project evaluates the performance characteristics and scalability of processes (using `fork()`) versus threads (using `pthread`). It includes:

* **Program A:** A process-based implementation that creates $N$ child processes.
* **Program B:** A thread-based implementation that creates $N$ worker threads.
* **Worker Functions:** Three distinct routines—`cpu` (computationally intensive), `mem` (memory allocation/manipulation intensive), and `io` (disk write intensive)—to test system bottlenecks.
* **Automation:** A shell script that automates execution, measures system metrics using `top`, `iostat`, and `time`, and exports the data to CSV files.

## 2. Files in the Submission

All files are located in the root directory as per the requirements:

* `MT25xxx_Part_A_Program_A.c`: C source for the process-based implementation.
* `MT25xxx_Part_A_Program_B.c`: C source for the thread-based implementation.
* `MT25xxx_Part_B_workers.h`: Header file containing the three worker functions (`cpu`, `mem`, `io`).
* `MT25xxx_Part_C_shell.sh`: Automation script for measurements and scalability tests.
* `MT25xxx_Part_C_CSV.csv`: Output data for the 2-worker combinations.
* `MT25xxx_Part_D_CSV.csv`: Output data for the scaling tests ($N=2 \dots 8$).
* `MT25xxx_Report.pdf`: Comprehensive analysis and discussion of observations.
* `Makefile`: Compilation script (excludes binary generation in final zip).
* `Readme`: This file.

## 3. Compilation and Execution

To compile the programs, ensure you are in the project directory and run:

```bash
make
```

This will generate the executables `prog_a` and `prog_b`.

To run the automated experiment and generate the CSV data:

```bash
chmod +x MT25xxx_Part_C_shell.sh
sudo ./MT25xxx_Part_C_shell.sh
```

**Note:** `sudo` is recommended because `iostat` and `taskset` may require elevated permissions for accurate hardware performance counters.

To clean up the environment and remove temporary files/binaries:

```bash
make clean
```

## 4. System Requirements

* **Operating System:** Linux (tested on Ubuntu 22.04/24.04 or WSL2).
* **Dependencies:** `gcc`, `sysstat` (for `iostat`), `bc` (for floating-point math in shell), `time`.
* **Hardware:** The script assumes a disk identifier like `sda` or `nvme`.

## 5. Logic Details

* **Iterations:** As the last digit of the roll number is 0, the worker functions use a multiplier of 9, resulting in $9 \times 10^3$ loop iterations.
* **Process/Thread Count:** As per instructions, "2 processes/threads" refers to those created in addition to the parent process or main thread.
