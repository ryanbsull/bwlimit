#include "bwpool.h"
#include <stdio.h>
#define BW_LIMIT 100

int main(){
    //int bwpool_fd = bwpool_create(0, 100,1,"test.log",LIMIT_READ);
	FILE *fp;
	fp = fopen("buf_test/test.buf", "w+");
	fprintf(fp, "This is a test\n");
	fputs("This is another test\n", fp);
	fclose(fp);
}
