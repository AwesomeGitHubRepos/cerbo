ASAN = -no-pie -fsanitize=address

.PHONY: all clean

all: blang2

clean :
	rm -f blang2 *.o

blang2 : blang2.cc
	g++ -std=gnu++1z -ggdb -O0 $(ASAN) $< -o $@
