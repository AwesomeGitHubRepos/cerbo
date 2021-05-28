#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	double sample_rate = 16000.0; // in Hz
	double fc = 400.0; // cut-off frequency in Hz
	double sample_secs = 120; // how many seconds to produce

	FILE *fp = fopen("rcfilter.raw", "wb");
	assert(fp);
	double PI = 3.14159265;
	double dt = 1.0/sample_rate;
	double vc = 0; // voltage across cap 
	srand(time(NULL));
	for(int t=0;t<sample_secs * sample_rate; ++t) {		
		uint8_t va = rand() % 256; // instaneous input voltage: white noise
		vc = vc + 2.0 * PI * fc * dt * ((double)va - vc); // derived from db05.296
		uint8_t pcm = vc;
		fwrite(&pcm, 1, sizeof(pcm), fp);
	}
	fclose(fp);
}
