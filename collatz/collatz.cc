#include <stdio.h>

typedef long int val_t;

int main()
{
	val_t max0 = 0;
	for(val_t i= 1 ; i<10'000'000; ++i) {
		val_t v = i;
		val_t max = i;
		while(v != 1) {
			if( v % 2 == 0) 
				v = v/2;
			else
				v =3*v+1;
			if(v>max) max = v;
		}
		if(max > max0) {
			printf("%ld\t%ld\n", i, max);
			max0 = max;
		}
	}

	return 0;
}
