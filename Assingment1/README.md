# Performance Benchmarking Analysis

This repository contains tools to visualize and analyze the performance metrics of `Program_A` and `Program_B`. The analysis focuses on resource utilization and execution time under varying parallel workloads.

## 📊 Overview

The benchmarking evaluates several key performance indicators (KPIs) across different worker configurations (2 to 8 workers):

* **CPU Utilization (%)**: Measures processing intensity.
* **Memory Usage (%)**: Tracks RAM consumption.
* **I/O Throughput (kB/s)**: Monitors disk read/write operations.
* **Execution Time (s)**: Evaluates overall efficiency and scaling.

## 🚀 Quick Start

### Prerequisites

Ensure you have Python installed along with the following libraries:
```bash
pip install pandas matplotlib seaborn
```

### Usage

1. Place your data in a file named `MT25083_PART_D_CSV.csv` in the root directory.
2. Run the visualization script:
```bash
python analyze_performance.py
```

## 📈 Key Insights from the Data

### 1. CPU vs. Memory Bottlenecks

The data reveals a stark difference in how the two programs handle resources. While `Program_A` shows decreasing CPU percentage as workers increase, `Program_B` maintains a high CPU load, indicating a more aggressive threading model.

### 2. The Swapping Threshold

The most significant performance degradation occurs in the `mem` functions at 8 workers. You will notice a massive spike in IO_Read and IO_Write values (exceeding 60,000 kB/s). This is a classic indicator of Disk Swapping, where physical memory is exhausted and the system begins using the hard drive as virtual memory.

### 3. Scaling Efficiency

The `Time(s)` metric shows that increasing the number of workers does not always lead to faster execution. In many cases, the overhead of synchronization or resource contention causes the execution time to increase linearly with the number of workers.

## 📂 Project Structure

* `MT25083_PART_D_CSV.csv`: Raw benchmark data.
* `analyze_performance.py`: Python script for generating grouped bar charts.
* `performance_comparison.png`: Generated visualization output.