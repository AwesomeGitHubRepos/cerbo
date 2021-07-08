#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>


// get time in milliseconds
long long millis()
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}


static bool done = false;

void task(void)
{
	static long long start, now;
	static void* where = &&start;	
	goto *where;
start:
	puts("Task started");
	start = millis();
	where = &&pausing;
	return;
pausing:
	if(millis() - start < 2000) return;
	puts("That's enough waiting");
	where = &&finis;
	return;
finis:
	done = true;
	puts("Task finished");
	return;
}

int main()
{
	while(!done) {
		task();		
	}
	puts("KTHXBYE");

	return 0;
}
