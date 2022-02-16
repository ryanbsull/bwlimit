#include "bwpool.h"
#include <stdio.h>
#define BW_LIMIT 100

int main(){
    //int bwpool_fd = bwpool_create(0, 100,1,"test.log",LIMIT_READ);
	FILE *fp;
	int i = 0;
	fp = fopen("buf_test/test.buf", "w+");
	for(; i < 100; i++){
		fputs("This is a test\n", fp);
	}
	fclose(fp);
}
