#include "bwpool.h"
#include <stdio.h>
#include <stdlib.h>

int bwpool_fd = 0;

int bwpool_create(int flags, unsigned long bytes, unsigned int interval_sec, char* lim_file, int limit_op){
    FILE *buf_ptr = fopen(BUFFER, "w");
    fprintf(buf_ptr, "c,%d,%ld,%d,%s,%d\n", flags, bytes, interval_sec, lim_file, limit_op);
    fclose(buf_ptr);
    return ++bwpool_fd;
}