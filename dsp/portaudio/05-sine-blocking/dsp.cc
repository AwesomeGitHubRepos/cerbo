#include <assert.h>
#include <iostream>
#include <sndfile.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <portaudio.h>
#include <atomic>
#include <string.h>

#define BLOCKING 1

static sem_t sem;

using namespace std;

#define FPB 512 // frames per buffer

//typedef paInt16 dtype;

SNDFILE* sndfile = nullptr;

void _check(int line, PaError err)
{
	if(err == paNoError) return;
	const char* msg = Pa_GetErrorText(err);
	printf("Failed:%d:%s\n", line, msg);
	exit(1);
}


#define CHECK() _check(__LINE__, paerr);

atomic<int> playing{0};

short buff0[FPB];
short buff1[FPB];

PaError paerr;
PaStream* strm;

void reader()
{
	static int i = 0;
	while(1) {
		//printf("Worker %d\n", i++);
		sem_wait(&sem);
		short* buff = buff0;
		if(playing == 0) buff = buff1;
		sf_count_t nread = sf_readf_short(sndfile, buff, FPB);
		putchar('.');
	}
}

int callback(const void* ibuffer, void *obuffer, unsigned long fpb, 
		const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
		void* userData)
{
	assert(obuffer != NULL);
	short* buff = buff0;
	if(playing==1) buff = buff1;
	memcpy(obuffer, buff, FPB);
	playing = 1 - playing;
	//putchar('-');
	sem_post(&sem);

	return paContinue;
}

void do_nonblocking()
{

	thread th(reader);
	sem_init(&sem, 0, 0);

	paerr = Pa_OpenDefaultStream(&strm, 
			0, // number input channels
			1, // number output channels
			paInt16, // sample format: signed 16 bit format
			44100.0, // sample rate
			FPB, // frames per buffer
			callback,
			NULL);
	assert(paerr == paNoError);
	paerr = Pa_StartStream(strm);
	if(paerr != paNoError) {
		const char* msg = Pa_GetErrorText(paerr);
		printf("Pa_StartStream error : %s\n", msg);
		exit(1);
	}
	//assert(paerr == paNoError);



	while(1) {
		//Pa_Sleep(2000);
		sleep(2);
		//sem_post(&sem);
	}

	th.join();
}

void do_blocking()
{
	paerr = Pa_OpenDefaultStream(&strm, 
			0, // number input channels
			1, // number output channels
			paInt16, // sample format: signed 16 bit format
			44100.0, // sample rate
			FPB, // frames per buffer
			NULL, // callback 0 implies blocking
			NULL);
	CHECK();
	paerr = Pa_StartStream(strm);
	CHECK();

	short buff[FPB*2];
	while(sf_count_t nread = sf_readf_short(sndfile, buff, FPB)) {
		// just take channel 0
		for(int i = 0; i < FPB; ++i)
			buff[i] = buff[i*2+1];
		paerr = Pa_WriteStream(strm, buff, FPB);
		CHECK();
	}


}

int main()
{
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
	paerr = Pa_Initialize();
	assert(paerr == paNoError);
#if BLOCKING
	do_blocking();
#else
	do_nonblocking();
#endif


	sf_close(sndfile);
	Pa_StopStream(strm);
	Pa_CloseStream(strm);
	paerr = Pa_Terminate();
	assert(paerr == paNoError);
	return 0;
}
