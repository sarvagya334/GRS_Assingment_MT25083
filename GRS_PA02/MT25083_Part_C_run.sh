#!/bin/bash

PORT=9000
TIME=10

SIZES="256 512 1024 4096"
THREADS="1 2 4 8"
IMPLS="A1 A2 A3"

mkdir -p results

echo "impl,size,threads,throughput,latency,cycles,cache_misses,context_switches" > results/results.csv

echo "Compiling programs..."
gcc ./MT25083_PART_A1_server.c -o server_A1 -lpthread
gcc ./MT25083_PART_A2_server.c -o server_A2 -lpthread
gcc ./MT25083_PART_A3_server.c -o server_A3 -lpthread
gcc ./MT25083_PART_A_client.c -o client -lpthread

echo "Starting experiments..."

for impl in $IMPLS
do
    for size in $SIZES
    do
        for th in $THREADS
        do
            echo "--------------------------------------"
            echo "Running $impl | size=$size | threads=$th"

            # start server
            ./server_$impl $PORT $size > server_log.txt 2>&1 &
            SERVER_PID=$!

            sleep 2

            # run client with perf
            perf stat -e cycles,cache-misses,context-switches \
            ./client 127.0.0.1 $PORT $th $size $TIME \
            > client_out.txt 2> perf_out.txt

            # stop server
            kill $SERVER_PID
            sleep 1

            # read client output
            THR=$(grep THROUGHPUT client_out.txt | cut -d'=' -f2)
            LAT=$(grep LATENCY client_out.txt | cut -d'=' -f2)

            # read perf output
            CYC=$(grep cycles perf_out.txt | awk '{print $1}' | tr -d ',')
            MIS=$(grep cache-misses perf_out.txt | awk '{print $1}' | tr -d ',')
            CSW=$(grep context-switches perf_out.txt | awk '{print $1}' | tr -d ',')

            # write to csv
            echo "$impl,$size,$th,$THR,$LAT,$CYC,$MIS,$CSW" >> results/results.csv

        done
    done
done

rm -f client_out.txt perf_out.txt server_log.txt

echo "Experiments finished."
echo "Results saved in results/results.csv"
