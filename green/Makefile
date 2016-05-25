PREFIX = $(HOME)/.local

.PHONY : all clean install

all : green

clean : 
	rm -rf *.o green

install :
	mkdir -p $(PREFIX)/bin
	cp green $(PREFIX)/bin

green : green.c
	gcc -std=gnu99 -o green green.c
