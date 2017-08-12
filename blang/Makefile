.PHONY: all clean

all : blang

clean :
	rm -f blang

blang : blang.cc
	g++ -g -O0 -lefence $< -o $@
