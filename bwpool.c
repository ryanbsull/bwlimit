#include "bwpool.h"
#include <stdio.h>
#include <stdlib.h>

int bwpool_create(int flags, unsigned long bytes, unsigned int interval_sec, int limit_op){
    FILE *buf_ptr = fopen(BUFFER, "w");
    fprintf(buf_ptr, "%d,%ld,%d,%d\n", flags, bytes, interval_sec, limit_op);
    fclose(buf_ptr);
}