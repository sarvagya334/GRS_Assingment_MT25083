#!/bin/bash

# --- Configuration ---
CORES="0"                # Restricted to 1 core for high contention
OUTPUT="MT25083_PART_D_CSV.csv"
PROGS_A=("MT25083_Part_A_Program_A_cpu" "MT25083_Part_A_Program_A_io" "MT25083_Part_A_Program_A_mem")
PROGS_B=("MT25083_Part_A_Program_B_cpu" "MT25083_Part_A_Program_B_io" "MT25083_Part_A_Program_B_mem")

# Clean up and write Header
rm -f $OUTPUT top.tmp io_raw.tmp
echo "Program+Function,Workers,CPU%,Mem%,IO_Read(kB/s),IO_Write(kB/s),Time(s)" > $OUTPUT

# --- Helper Function to Run and Measure ---
run_benchmark() {
    local prog=$1
    local n=$2
    
    echo "Running $prog with $n workers on core $CORES..."

    # 1. Start monitors
    # IMPORTANT: Added -c to top to prevent name truncation
    top -c -b -d 0.1 -n 300 | grep --line-buffered "$prog" > top.tmp &
    TOP_PID=$!
    iostat -dk 1 60 > io_raw.tmp &
    IO_PID=$!

    # 2. Execution
    START=$(date +%s.%N)
    taskset -c $CORES ./$prog $n > /dev/null 2>&1
    END=$(date +%s.%N)

    # 3. Cleanup
    kill $TOP_PID $IO_PID 2>/dev/null
    sleep 0.5 

    # 4. Calculate stats
    # Use perl to avoid "bc not found" errors
    RUNTIME=$(perl -e "print $END - $START")
    
    # Check if top.tmp actually has data before averaging
    if [ -s top.tmp ]; then
        AVG_CPU=$(awk '{sum+=$9; count++} END {print (count>0 ? sum/count : 0)}' top.tmp)
        AVG_MEM=$(awk '{sum+=$10; count++} END {print (count>0 ? sum/count : 0)}' top.tmp)
    else
        AVG_CPU=0
        AVG_MEM=0
    fi
    
    IO_R=$(awk '$3 ~ /^[0-9.]+$/ {r+=$3; c++} END {print (c>0 ? r/c : 0)}' io_raw.tmp)
    IO_W=$(awk '$4 ~ /^[0-9.]+$/ {w+=$4; c++} END {print (c>0 ? w/c : 0)}' io_raw.tmp)

    # 5. Append results
    echo "$prog,$n,$AVG_CPU,$AVG_MEM,$IO_R,$IO_W,$RUNTIME" >> $OUTPUT
}

# --- Execution ---
# Run A and B variants for the full range of 2-8 workers
for p in "${PROGS_A[@]}" "${PROGS_B[@]}"; do
    for n in {2..8}; do
        run_benchmark $p $n
    done
done

echo "------------------------------------------------"
echo "Full Benchmark Complete. Results: $OUTPUT"