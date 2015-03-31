
.PHONY : all clean install

all : green

clean : 
	rm -rf *.o green

install :
	cp green /usr/local/bin

green : green.c
	gcc -std=gnu99 -o green green.c
