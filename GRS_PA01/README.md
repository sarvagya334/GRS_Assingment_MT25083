# Readme

## 1. Description

This project evaluates the performance characteristics and scalability of processes (using `fork()`) versus threads (using `pthread`). The system is tested under three specific workloads:

* **CPU:** Computationally intensive mathematical operations.
* **Memory:** High-frequency allocation and memory manipulation.
* **I/O:** Sequential disk write operations.

The suite measures CPU saturation, Memory footprint, and I/O throughput across varying worker counts (2 to 8), specifically observing the effects of resource pinning on a single CPU core.

## 2. Files in the Submission

All files are located in the root directory as per the requirements:

* `MT25083_Part_A_Program_A_[cpu/io/mem].c`: Source files for process-based variants.
* `MT25083_Part_A_Program_B_[cpu/io/mem].c`: Source files for thread-based variants.
* `MT25083_PART_C_workers.h`: Shared worker function definitions.
* `MT25083_PART_C_SHELL.sh`: Automation script for Part C (Table of 6 combinations).
* `MT25083_PART_D_SHELL.sh`: Automation script for Part D (Scalability testing).
* `MT25083_PART_D_plot_generate.py`: Python script for generating performance graphs.
* `Makefile`: Advanced build script for compilation and experiment automation.
* `MT25083_Report.pdf`: Final analysis of observed data.

## 3. Compilation and Execution

The `Makefile` has been configured to handle the entire lifecycle of the experiment.

### Step 1: Compilation

To compile all 6 program variants and object files:

```bash
make
```

### Step 2: Run Part C (Measurement Table)

To execute the baseline benchmarks and generate `final_resultsC.csv`:

```bash
make resultsC
```

### Step 3: Run Part D (Scalability & Plotting)

To run the scalability tests (2-8 workers), generate `final_resultsD.csv`, and automatically create the performance plots:

```bash
make fullD
```

(Alternatively, use `make resultsD` to just get the CSV, or `make plot` if the CSV already exists).

### Step 4: Cleanup

To remove binaries and object files:

```bash
make clean
```

To remove generated CSV results:

```bash
make cleancsv
```

## 4. System Requirements

* **Operating System:** Linux (tested on Ubuntu 22.04/24.04 or WSL2).
* **Dependencies:** `gcc`, `sysstat` (for `iostat`), `bc` (for floating-point math in shell), `time`, Python 3 with `pandas`, `matplotlib`, and `seaborn`.
* **Hardware:** The script assumes a disk identifier like `sda` or `nvme`.
* **Permissions:** Some commands may require `sudo` for accurate hardware performance counters.