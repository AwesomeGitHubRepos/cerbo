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
#include <stdio.h>
#include <thread>

#include "gui.h"


using namespace std;
using namespace std::chrono;

#define FPB 512

constexpr float sample_freq = 44000.0;
atomic<float> noise_freq{8000};

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


atomic<bool> keep_generating{true};
atomic<bool> output_square_wave{false};

void generate()
{
	float  buff[FPB];
	static atomic<float> local_noise_freq{noise_freq.load()};
	float sample_every = sample_freq / noise_freq;
	int sample_num = 0;
	//auto sample_value = rand() < RAND_MAX/2 ? 1.0 : 0.0;
	float sample_value = 0;
	while(keep_generating) {
		if(local_noise_freq != noise_freq) {
			local_noise_freq = noise_freq.load();
			sample_num = 0;
			sample_every = sample_freq / noise_freq;
		}
		bool sqwave = output_square_wave;
		for(int i = 0; i< FPB; ++i) {
			if(sqwave) {
				sample_value = sample_num < sample_every/2 ? 1.0 : -1.0;
			} else if(sample_num == 0) { // maybe select a random noise value
				sample_value = rand() < RAND_MAX/2 ? 1.0 : -1.0;
			}

			buff[i] = sample_value;
			sample_num++;
			if(sample_num >= sample_every) sample_num = 0;

		}
		paerr = Pa_WriteStream(strm, buff, FPB);
		CHECK();
		//this_thread::sleep_for(std::chrono::microseconds(20));
	}

}


void slider_callback(Fl_Value_Slider* slider, void* data)
{
	noise_freq = slider->value();
	//printf("Slider value:%f\n", noise_freq);
	//return 0;
}

void square_changed(Fl_Check_Button* btn, void* data)
{
	output_square_wave = btn->value();
}



void gui_thread()
{
	Fl_Double_Window* root = make_window();
	root->show();
	auto res = Fl::run();
	keep_generating = false;

}

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


	thread th(generate);
	thread th_gui(gui_thread);

	while(keep_generating) this_thread::sleep_for(std::chrono::milliseconds(1000));



	th.join();
	th_gui.join();
	Pa_StopStream(strm);
	Pa_CloseStream(strm);
	paerr = Pa_Terminate();
	CHECK();

	return 0;
}
