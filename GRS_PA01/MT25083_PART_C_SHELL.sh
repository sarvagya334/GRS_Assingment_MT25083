#!/bin/bash

# --- Configuration ---
CORES="0"                # Restricted to 2 cores
OUTPUT="MT25083_PART_C_CSV.csv"
PROGRAMS=("MT25083_Part_A_Program_A_cpu" "MT25083_Part_A_Program_A_io" "MT25083_Part_A_Program_A_mem" "MT25083_Part_A_Program_B_cpu" "MT25083_Part_A_Program_B_io" "MT25083_Part_A_Program_B_mem")

# Clean up and write Header exactly as requested
rm -f $OUTPUT top_raw.tmp io_raw.tmp
echo "Program+Function,CPU%,Mem%,IO_Read(kB/s),IO_Write(kB/s),Time(s)" > $OUTPUT

# --- Helper Function ---
run_benchmark() {
    local prog_name=$1
    local workers=5
    # Fixing the name check logic
    if [[ $prog_name == *Program_B* ]]; then workers=5; fi

    echo "Running $prog_name with $workers workers on cores $CORES..."

    # 1. ADDED -c FLAG TO SHOW FULL COMMAND NAME
    top -c -b -d 0.1 -n 200 | grep --line-buffered "$prog_name" > top_raw.tmp &
    TOP_PID=$!
    iostat -dk 1 30 > io_raw.tmp &
    IO_PID=$!

    # 2. Execution
    START_TIME=$(date +%s.%N)
    taskset -c $CORES ./$prog_name $workers > /dev/null 2>&1
    END_TIME=$(date +%s.%N)

    # 3. Cleanup
    kill $TOP_PID $IO_PID 2>/dev/null
    sleep 1 

    # 4. Aggregate Data
    AVG_CPU=$(awk '{sum+=$9} END {if (NR>0) print sum/NR; else print 0}' top_raw.tmp)
    AVG_MEM=$(awk '{sum+=$10} END {if (NR>0) print sum/NR; else print 0}' top_raw.tmp)
    
    IO_READ=$(awk '$3 ~ /^[0-9.]+$/ {r+=$3} END {if (NR>0) print r/NR; else print 0}' io_raw.tmp)
    IO_WRITE=$(awk '$4 ~ /^[0-9.]+$/ {w+=$4} END {if (NR>0) print w/NR; else print 0}' io_raw.tmp)
    
    DIFF_TIME=$(echo "$END_TIME - $START_TIME" | bc)

    # 5. Output to CSV
    echo "$prog_name,$AVG_CPU,$AVG_MEM,$IO_READ,$IO_WRITE,$DIFF_TIME" >> $OUTPUT
}

# --- Execution ---
for p in "${PROGRAMS[@]}"; do
    run_benchmark $p
done

echo "------------------------------------------------"
cat $OUTPUT
echo "------------------------------------------------"
echo "Results saved to $OUTPUT"