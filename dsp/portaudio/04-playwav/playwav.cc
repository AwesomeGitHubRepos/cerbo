#include <assert.h>
#include <iostream>
#include <sndfile.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <portaudio.h>



static sem_t sem;

using namespace std;

#define FPB 512 // frames per buffer

SNDFILE* sndfile = nullptr;


void reader()
{
	static int i = 0;
	while(1) {
		printf("Worker %d\n", i++);
		sem_wait(&sem);
	}
}

int callback(const void* ibuffer, void *obuffer, unsigned long fpb, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
		void* userData)
{
	assert(obuffer != NULL);

	return paContinue;
}

int main()
{
	thread th(reader);
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

	// init sound stream
	PaError paerr;
	paerr = Pa_Initialize();
	assert(paerr == paNoError);
	PaStream* strm;
	paerr = Pa_OpenDefaultStream(&strm, 
			0, // number input channels
			1, // number output channels
			paInt16, // sample format: signed 16 bit format
			44100, // sample rate
			FPB, // frames per buffer
			callback,
			NULL);
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
