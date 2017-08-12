.PHONY: all clean

all : blang

clean :
	rm -f blang

blang : blang.cc
	g++ -std=gnu++14 -g -O0 -lefence $< -o $@
