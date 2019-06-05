#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	FILE *fp = fopen("brown.raw", "wb");	
	assert(fp);
	//double PI = 3.14159265;
	typedef long double val_t;
	val_t sample_rate = 8000; // in Hz
	val_t dt = 1.0 / sample_rate;
	val_t sum = 0;	
	srand(time(NULL));
	int sample_secs = 10; // produce 10 seconds worth of data
	int arr_size = sample_rate * sample_secs;
	val_t ys[arr_size];

	// generate uniform distribution of values from -1 to 1
	for(int i=0; i< arr_size; ++i) {
		val_t r = (val_t)(rand() % 10000 - 5000)/5000.0;
		ys[i] = r; // / (val_t) arr_size; // we use arr_size to scale it to prevent overflow
	}

	// accumulate them
	val_t min = ys[0], max = ys[0];
	for(int i=1; i< arr_size; ++i) {
		ys[i] += ys[i-1];
		if(ys[i] < min) min = ys[i];
		if(ys[i] > max) max = ys[i];
	}

	// normalise it
	for(int i=0; i< arr_size; ++i) {
		ys[i] = (ys[i] -min) / (max-min);
		printf("%Lf\n", ys[i]);
	}

	
	// now output values
	for(int i=0; i< arr_size; ++i) {
		val_t pcm1 = ys[i]*255.0;
		uint8_t pcm = pcm1;
		fwrite(&pcm, 1, 1, fp);
	}


	fclose(fp);
}
