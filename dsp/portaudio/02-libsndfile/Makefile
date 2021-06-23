wave : wave.cc
	g++ $^ -o $@ -lsndfile

out.raw : wave
	./wave

play : out.raw
	aplay -f S16_LE -r 44100 out.raw

