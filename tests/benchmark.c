#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "cirbuf.h"

#define BUFFER_SIZE 1024
#define MESSAGE_SIZE 32

static inline double microtime() {
    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
    return tv.tv_nsec * 1e-3;
}

int main(int argc, char** argv) {
    uint8_t message[MESSAGE_SIZE];
    uint8_t buffer[BUFFER_SIZE];
    size_t offset;
    size_t runs = atoi(argv[1]);
 
    long slow_start = microtime();
    offset = 0;
    for(size_t i = 0; i < runs; i++){
        long mask = (MESSAGE_SIZE - BUFFER_SIZE + offset) >> 63;
        const size_t part1 = MESSAGE_SIZE & mask + (BUFFER_SIZE - offset) & ~mask;
        const size_t part2 = MESSAGE_SIZE - part1;
        memcpy(buffer + offset, message, part1);
        memcpy(buffer, message + part1, part2);
        offset = (offset + MESSAGE_SIZE) % BUFFER_SIZE;
    }
    long slow_stop = microtime();
 
    cirbuf_t cb;
    cirbuf_new(&cb, BUFFER_SIZE);
    long fast_start = microtime();
    for(size_t i = 0; i < runs; i++){
       cirbuf_offer(&cb, message, MESSAGE_SIZE);
       cirbuf_poll(&cb, MESSAGE_SIZE);
    }
    long fast_stop = microtime();
 
    printf("%ld %ld\n", (slow_stop - slow_start), (fast_stop - fast_start));
 
    return 0;
}
