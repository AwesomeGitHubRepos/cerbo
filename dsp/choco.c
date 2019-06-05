#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int rado(int x)
{
	return rand() % (2*x+1) - x;
}
int main()
{
	FILE *fp = fopen("choco.raw", "wb");	
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
	int val = 127;
	for(int i=0; i< arr_size; ++i) {
		int r = rado(64);
		//val += rado(6);
		if(val+r >255) val = val-r;
		else if(val+ r <0)   val = val -r;
		else val +=r;
		uint8_t pcm = val;
		fwrite(&pcm, 1, 1, fp);
	}

	fclose(fp);
}
