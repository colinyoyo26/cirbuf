#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "cirbuf.h"
#include "offer_imprv.h"

#define BUFFER_SIZE 65536
#define MESSAGE_SIZE message_size
#define RUNS 1000

size_t message_size;

static inline double microtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return 1e6 * tv.tv_sec + tv.tv_usec;
}

int main(int argc, char** argv) {
    uint8_t message[MESSAGE_SIZE];
    uint8_t buffer[BUFFER_SIZE];
    size_t offset;
    message_size = atoi(argv[1]);
 
    double slow_start = microtime();
    offset = 0;
    for(size_t i = 0; i < RUNS; i++){
        long mask = (MESSAGE_SIZE - BUFFER_SIZE + offset) >> 63;
        const size_t part1 = MESSAGE_SIZE & mask + (BUFFER_SIZE - offset) & ~mask;
        const size_t part2 = MESSAGE_SIZE - part1;
        memcpy(buffer + offset, message, part1);
        memcpy(buffer, message + part1, part2);
        offset = (offset + MESSAGE_SIZE) % BUFFER_SIZE;
    }
    double slow_stop = microtime();
 
    cirbuf_t cb;
    cirbuf_new(&cb, BUFFER_SIZE);
    double fast_start = microtime();
    for(size_t i = 0; i < RUNS; i++){
       cirbuf_offer(&cb, message, MESSAGE_SIZE);
       cirbuf_poll(&cb, MESSAGE_SIZE);
    }
    double fast_stop = microtime();
 
    cirbuf_t cb2;
    cirbuf_new(&cb2, BUFFER_SIZE);
    double opt_start = microtime();
    for(size_t i = 0; i < RUNS; i++){
       cirbuf_offer_opt(&cb2, message, MESSAGE_SIZE);
       cirbuf_poll(&cb2, MESSAGE_SIZE);
    }
    double opt_stop = microtime();
 
    printf("%lf %lf %lf\n", (slow_stop - slow_start),
                            (fast_stop - fast_start),
                            (opt_stop - opt_start));
 
    return 0;
}
