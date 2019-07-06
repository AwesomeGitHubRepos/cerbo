#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	FILE *fp = fopen("white.raw", "wb");
	assert(fp);
	//double PI = 3.14159265;
	//double sample_rate = 8000; // in Hz
	//double freq = 440; // in  Hz
	srand(time(NULL));
	int sample_secs = 10; // produce 10 seconds worth of data
	//for(int t=0;t<sample_secs * sample_rate; ++t) {		
	for(int t=0;t<131072; ++t) {		
		uint8_t pcm = rand() % 256;
		fwrite(&pcm, 1, sizeof(pcm), fp);
		printf("%d\n", pcm);
	}
	fclose(fp);
}
