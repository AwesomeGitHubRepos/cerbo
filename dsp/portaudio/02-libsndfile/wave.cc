#include <sndfile.h>

int main()
{
	SF_INFO sfinfo;
	SNDFILE* sndfile = sf_open("/home/pi/Music/sheep.wav", SFM_READ, &sfinfo);

	sf_close(sndfile);
	return 0;
}
