ASAN = -no-pie -fsanitize=address

.PHONY: all clean

all: blang2

clean :
	rm -f blang2

blang2 : blang2.cc
	g++ -std=gnu++14 -ggdb -O0 $(ASAN) $< -o $@
