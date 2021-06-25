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
#include <cstdlib>
#include <ctime>



using namespace std;
using namespace std::chrono;

#define FPB 512

constexpr float sample_freq = 44000.0;

static_assert(sizeof(float) == 4);

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

	srand((unsigned int)time(NULL));
	float  buff[FPB];
	while(1) {
		for(int i = 0; i< FPB; ++i) {
			buff[i] = (float(rand())/float((RAND_MAX))) * 2.0 - 1.0;
		}
		paerr = Pa_WriteStream(strm, buff, FPB);
		CHECK();
	}

	Pa_StopStream(strm);
	Pa_CloseStream(strm);
	paerr = Pa_Terminate();
	CHECK();
	return 0;
}
