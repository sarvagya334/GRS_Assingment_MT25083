# Roll No: MT25083

CC = gcc
CFLAGS = -Wall -O2 -pthread

# targets
all: server_A1 server_A2 server_A3 client

# A1
server_A1: MT25083_PART_A1_server.c
	$(CC) $(CFLAGS) MT25083_PART_A1_server.c -o server_A1

# A2
server_A2: MT25083_PART_A2_server.c
	$(CC) $(CFLAGS) MT25083_PART_A2_server.c -o server_A2

# A3
server_A3: MT25083_PART_A3_server.c
	$(CC) $(CFLAGS) MT25083_PART_A3_server.c -o server_A3

# client
client: MT25083_PART_A_client.c
	$(CC) $(CFLAGS) MT25083_PART_A_client.c -o client

# run all experiments
run: all
	bash ./MT25083_Part_C_run.sh

# remove binaries
clean:
	rm -f server_A1 server_A2 server_A3 client
	rm -f *.o
	rm -rf results
	rm -f client_out.txt perf_out.txt server_log.txt

.PHONY: all run clean
