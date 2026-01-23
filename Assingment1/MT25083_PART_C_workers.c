#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MT25083_PART_C_workers.h"

void* cpu() {

    // Using double multiplication/division is harder on the CPU than integer addition
    volatile double a = 10.5;
    for(long long i = 0; i < 500000000; i++) {
        a = (a * 1.000001) / 1.000001; 
    }
    return NULL;
}

void* mem() {
    // 500MB is large enough to show up in top
    size_t size = 500 * 1024 * 1024; 
    char *buffer = malloc(size);
    if (!buffer) return NULL;

    for(int loops = 0; loops < 10; loops++) {
        // memset ensures the OS actually maps the physical RAM (Resident Set Size)
        memset(buffer, 'A', size); 
        for(size_t i = 0; i < size; i += 4096) {
            buffer[i] = (char)(i % 256);
        }
        sleep(1); // Give 'top' a chance to catch the allocation
    }

    free(buffer);
    return NULL;
}

void* io() {
    FILE *fp = fopen("temp_io_test.bin", "wb+");
    if (!fp) return NULL;
    char data[8192];
    memset(data, 'D', sizeof(data));
    for(int i = 0; i < 1000; i++) {
        fwrite(data, 1, sizeof(data), fp);
        if (i % 50 == 0) fflush(fp); 
    }
    fclose(fp);
    unlink("temp_io_test.bin");
    return NULL;
}