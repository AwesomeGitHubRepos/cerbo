#include <assert.h>
#include <iostream>
//#include <sndfile.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>



static sem_t sem;

using namespace std;

void worker()
{
	static int i = 0;
	while(1) {
		printf("Worker %d\n", i++);
		sem_wait(&sem);
	}
}

int main()
{
	thread th(worker);
	sem_init(&sem, 0, 0);

	while(1) {
		sleep(2);
		sem_post(&sem);
	}

	th.join();
	return 0;
}
