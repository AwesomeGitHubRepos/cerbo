BIN = dsp

$(BIN) : dsp.cc
	g++ $^ -o $@  -lpthread -lportaudio -lm

clean:
	rm -f $(BIN)
