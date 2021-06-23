#include <iostream>
#include <sndfile.h>

using namespace std;

int main()
{
	SF_INFO sfinfo;
	sfinfo.format = 0;
	SNDFILE* sndfile = sf_open("/home/pi/Music/sheep.wav", SFM_READ, &sfinfo);
	cout << "Sample rate: " << sfinfo.samplerate << "\n";
	cout << "Channels: " << sfinfo.channels << "\n";
	printf("Format: 0x%X, ", sfinfo.format);
	cout << "Wave file?: " << ((SF_FORMAT_WAV>>2) == (sfinfo.format>>2)) << "\n";

	sf_close(sndfile);
	return 0;
}
