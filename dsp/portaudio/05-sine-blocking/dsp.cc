#include <assert.h>
#include <iostream>
#include <sndfile.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <portaudio.h>
#include <atomic>
#include <string.h>
#include <math.h>
#include <chrono>


#define BLOCKING 1

static sem_t sem;

using namespace std;
using namespace std::chrono;

#define FPB 512
//#define FPB (512*4)

static_assert(sizeof(float) == 4);

constexpr float sample_freq = 4000.0;
constexpr float dt = 1.0/sample_freq;
constexpr float sine_freq = 440.0; // Hz
constexpr float pi = 3.1412;
constexpr float w = 2.0 * pi * sine_freq; // angular frequency
//typedef paInt16 dtype;


void _check(int line, PaError err)
{
	if(err == paNoError) return;
	const char* msg = Pa_GetErrorText(err);
	printf("Failed:%d:%s\n", line, msg);
	exit(1);
}


#define CHECK() _check(__LINE__, paerr);


PaError paerr;
PaStream* strm;

int main()
{
	// init sound stream
	paerr = Pa_Initialize();
	CHECK();
	paerr = Pa_OpenDefaultStream(&strm, 
			0, // number input channels
			1, // number output channels
			paFloat32,
			sample_freq, // sample rate
			FPB, // frames per buffer
			NULL, // callback 0 implies blocking
			NULL);
	CHECK();
	paerr = Pa_StartStream(strm);
	CHECK();

	int nblocks = 1000;
	float  buff[FPB*nblocks];
	auto start = high_resolution_clock::now();
	float t = 0;
	for(int i = 0; i< FPB*nblocks; ++i) {
		buff[i] = sin(w*t);
		t+= dt;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	 cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

	/*
	   while(sf_count_t nread = sf_readf_short(sndfile, buff, FPB)) {
	// just take channel 0
	for(int i = 0; i < FPB; ++i)
	buff[i] = buff[i*2+1];
	}
	*/

	for(int i = 0; i<nblocks; ++i) {
			paerr = Pa_WriteStream(strm, buff + i*FPB, FPB);
			CHECK();
	}
#if 0
	while(1) {
		for(int i = 0; i< FPB; ++i) {
			buff[i] = sin(w * t);
			t += dt;
			paerr = Pa_WriteStream(strm, buff, FPB);
			CHECK();
		}

		t = fmod(t, 1.0/sine_freq); 
	}
#endif

	Pa_StopStream(strm);
	Pa_CloseStream(strm);
	paerr = Pa_Terminate();
	CHECK();
	return 0;
}
