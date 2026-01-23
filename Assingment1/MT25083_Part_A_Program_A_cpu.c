#include <stdio.h>
#include <stdlib.h>  // Required for atoi()
#include <unistd.h>
#include <sys/wait.h>
#include "MT25083_PART_C_workers.h"

int main(int argc, char *argv[]) {
    // Default to 5 if no argument is provided
    int n = (argc > 1) ? atoi(argv[1]) : 5;

    for(int i = 0; i < n; i++) {
        int pid = fork();
        if(pid == 0) {
            cpu();
            return 0; // Child must exit to avoid "fork bomb"
        }
    }

    // Parent waits for exactly n children
    for(int i = 0; i < n; i++) {
        wait(NULL);
    }
    
    return 0;
}