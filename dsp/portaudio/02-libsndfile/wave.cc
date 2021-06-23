#include <assert.h>
#include <iostream>
#include <sndfile.h>

using namespace std;

int main()
{
	SF_INFO sfinfo;
	sfinfo.format = 0;
	SNDFILE* sndfile = sf_open("/home/pi/Music/sheep.wav", SFM_READ, &sfinfo);
	assert(sndfile);
	cout << "Sample rate: " << sfinfo.samplerate << "\n";
	int nchannels = sfinfo.channels;
	cout << "Channels: " << nchannels << "\n";
	printf("Format: 0x%X, ", sfinfo.format);
	cout << "Wave file?: " << ((SF_FORMAT_WAV>>2) == (sfinfo.format>>2)) << "\n";
	printf("Sizeof short: %d\n", sizeof(short));

	FILE* fp = fopen("out.raw", "w");
	assert(fp);
	const int NSAMPLES = 512;
	short buf[NSAMPLES*nchannels];
	while(sf_count_t nread = sf_readf_short(sndfile, buf, NSAMPLES)) {
		for(int i = 0; i < nread; ++i) {
			// output only the first channel
			fwrite(buf + 2* i, 1, sizeof(short), fp);
		}
	}


	fclose(fp);
	sf_close(sndfile);
	return 0;
}
