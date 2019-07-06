#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* fp = fopen("noise.raw", "wb");
	assert(fp);
	for(int i = 0; i<80000; ++i) {
		int v = rand() % 256;
		fprintf(fp, "%c", v);
	}
	fclose(fp);
	return 0;
}
