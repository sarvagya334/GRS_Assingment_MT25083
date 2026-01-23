# Compiler to use
CC = gcc

# Compiler flags:
# -Wall    : show all warnings
# -pthread : enable pthread support (thread library)
CFLAGS = -Wall -pthread

# Header file dependency
# Any time this header changes, workers.o should be recompiled
DEPS = MT25083_PART_C_workers.h

# Object file that will be linked with all programs
# workers.c -> workers.o
OBJ = MT25083_PART_C_workers.o

# List of all final executable programs that must be built
# These are 6 programs (Process-based cpu/io/mem and Thread-based cpu/io/mem)
TARGETS = MT25083_Part_A_Program_A_cpu MT25083_Part_A_Program_A_io MT25083_Part_A_Program_A_mem \
          MT25083_Part_A_Program_B_cpu MT25083_Part_A_Program_B_io MT25083_Part_A_Program_B_mem

# Shell script for benchmarking suite D
SCRIPTT = MT25083_PART_D_SHELL.sh 

# Shell script for benchmarking suite C
SCRIPT = MT25083_PART_C_SHELL.sh 

# Python script to generate plots from CSV results (part D)
PYTHON_SCRIPT = MT25083_PART_D_plot_generate.py


# Default target:
# When you run `make`, it will build all targets in TARGETS
all: $(TARGETS)


# ---------------- PART C ----------------
# Custom target: resultsC
# It first compiles all targets (depends on 'all')
# Then it runs the Part C shell script and generates final_resultsC.csv
resultsC: all
	@chmod +x $(SCRIPT)                      # make script executable
	@echo "Starting Benchmark Suite C..."     # message
	./$(SCRIPT)                              # run script
	@echo "Finished. Results are in final_resultsC.csv"


# ---------------- PART D ----------------
# Custom target: resultsD
# Like resultsC but runs PART D script and makes final_resultsD.csv
resultsD: all
	@chmod +x $(SCRIPTT)                     # make script executable
	@echo "Starting Benchmark Suite D..."
	./$(SCRIPTT)                             # run script
	@echo "Finished. Results are in final_resultsD.csv"


# ---------------- PLOT GENERATION ----------------
# plot target:
# runs python script to generate plots from final_resultsD.csv
plot:
	@echo "Generating plots from final_resultsD.csv..."
	python3 $(PYTHON_SCRIPT)


# fullD target:
# does everything for part D in one command:
# compile -> run -> generate plots
fullD: resultsD plot


# ---------------- BUILD RULES ----------------
# Generic rule to compile any .c file into .o
# Example: workers.c -> workers.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	# $@ = output file (example: workers.o)
	# $< = input file  (example: workers.c)


# Rule to build executables in TARGETS
# Example: MT25083_Part_A_Program_A_cpu is built from:
# MT25083_Part_A_Program_A_cpu.c + workers.o
$(TARGETS): %: %.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	# $@ = target executable name
	# $^ = all dependencies (program.c + workers.o)


# ---------------- CLEANUP ----------------
# Remove executables, object files, and temporary files
clean:
	rm -f $(TARGETS) *.o *.tmp

# Remove all csv result files
cleancsv:
	rm -f *.csv
