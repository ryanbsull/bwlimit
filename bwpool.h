#ifndef BWPOOL_H
#define BWPOOL_H

#define     LIMIT_READ  0
#define     LIMIT_WRITE 1
#define     LIMIT_ALL   3
#define     BUFFER      "buf_test/buffer"

int bwpool_create(
    int flags, unsigned long bytes, unsigned int interval_sec, 
    char* lim_file, int limit_op
);

#endif