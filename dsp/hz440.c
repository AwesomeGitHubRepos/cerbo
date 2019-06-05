#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

int main()
{
	FILE *fp = fopen("hz440.raw", "wb");
	assert(fp);
	double PI = 3.14159265;
	double sample_rate = 8000; // in Hz
	double freq = 440; // in  Hz
	int sample_secs = 10; // produce 10 seconds worth of data
	for(int t=0;t<sample_secs * sample_rate; ++t) {
		uint8_t pcm = (uint8_t) (128.0 + 128.0 * sin((double) t * freq * 2.0 *PI/sample_rate));
		fwrite(&pcm, 1, sizeof(pcm), fp);

	}
	fclose(fp);
}
