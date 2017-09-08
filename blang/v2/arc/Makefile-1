AM_CXXFLAGS =
AM_CXXFLAGS += -Wfatal-errors
AM_CXXFLAGS += -no-pie -fsanitize=address

.PHONY: all clean

all: blang2

clean :
	rm -f blang2 *.o

blang2 : blang2.cc
	g++ -std=gnu++14 -ggdb -O0 $(AM_CXXFLAGS) $< -o $@
