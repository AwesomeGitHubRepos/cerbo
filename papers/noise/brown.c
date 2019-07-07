#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	const int n = 100 * 1000;
	float y[n];
	y[0] = 0; // the signal
	float min = 0, max = 0;
	for(int i = 1; i< n; ++i) {
		float white = (rand() % 7) - 3;
		y[i] = y[i-1] + white;
		if(y[i]>max) max = y[i];
		if(y[i]<min) min = y[i];
	}

	// output a normalisation of the array
	FILE* fp = fopen("noise.raw", "wb");
	assert(fp);
	for(int i =0; i< n; ++i) {
		int v = 255.0 * (y[i] - min)/(max-min);
		fprintf(fp, "%c", v);
	}
	fclose(fp);

	return 0;
}
