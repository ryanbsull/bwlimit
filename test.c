#include "bwpool.h"
#define BW_LIMIT 100

int main(){
    int bwpool_fd = bwpool_create(0, 100,1,"test.log",LIMIT_READ);
}