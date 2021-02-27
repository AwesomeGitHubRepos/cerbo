#include <math.h>
#include <stdio.h>

int main()
{
	double freq = 440.0, duration = 60.0, sampr = 8000.0;
	double dt = 1.0/sampr;

	FILE* fp = fopen("song.raw", "wb");
	for(double t = 0.0; t< duration; t += dt) {
		double pos = fmod(t,1.0/freq);
		int b = 255* (pos < 1.0/freq/2);
		fputc(b, fp);
	}

	fclose(fp);
	return 0;
}
