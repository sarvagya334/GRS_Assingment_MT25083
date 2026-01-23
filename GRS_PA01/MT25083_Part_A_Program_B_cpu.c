#include <pthread.h>   // For pthread_create(), pthread_join(), pthread_t
#include <stdio.h>     // For standard I/O functions (printf, etc.)
#include "MT25083_PART_C_workers.h"  // Your header file where cpu() is declared

int main(int argc, char *argv[]) {

    // n = number of threads to create
    // If user provides a command-line argument, use that
    // Example: ./a.out 6  -> n = 6
    // Otherwise default n = 5
    int n = (argc > 1) ? atoi(argv[1]) : 5;

    // Create an array to store thread IDs (pthread_t is a datatype for thread handles)
    pthread_t thread[n];

    // Loop to create n threads
    for (int i = 0; i < n; i++) {

        // pthread_create arguments:
        // 1) &thread[i]  -> where to store the created thread ID
        // 2) NULL        -> default thread attributes
        // 3) cpu         -> function the thread will run (from workers.h)
        // 4) NULL        -> argument passed to cpu()
        pthread_create(&thread[i], NULL, cpu, NULL);
    }

    // Wait for all threads to finish before exiting main
    for (int i = 0; i < n; i++) {

        // pthread_join blocks (waits) until thread[i] finishes
        pthread_join(thread[i], NULL);
    }

    // After all threads complete, program ends
    return 0;
}
