#include <assert.h>
#include <iostream>
#include <sndfile.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <portaudio.h>



static sem_t sem;

using namespace std;

SNDFILE* sndfile = nullptr;


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

	// open soundfile
	SF_INFO sfinfo;
	sfinfo.format = 0;
	sndfile = sf_open("/home/pi/Music/sheep.wav", SFM_READ, &sfinfo);
	assert(sndfile);
	cout << "Sample rate: " << sfinfo.samplerate << "\n";
	int nchannels = sfinfo.channels;
	cout << "Channels: " << nchannels << "\n";
	printf("Format: 0x%X, ", sfinfo.format);
	cout << "Wave file?: " << ((SF_FORMAT_WAV>>2) == (sfinfo.format>>2)) << "\n";
	printf("Sizeof short: %d\n", sizeof(short));

	PaError paerr;
	paerr = Pa_Initialize();
	assert(paerr == paNoError);

	while(1) {
		sleep(2);
		//sem_post(&sem);
	}

	th.join();
	sf_close(sndfile);
	paerr = Pa_Terminate();
	assert(paerr == paNoError);
	return 0;
}
