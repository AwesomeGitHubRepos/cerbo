#CFLAGS = -ggdb -fsanitize-address
CFLAGS = -g -O0 -Wall

CC = g++

all: forth

forth.o : forth.cpp
	$(CC) $(CFLAGS) -c $^ 

forth : forth.o
	$(CC) $^ -o $@

clean:
	rm -f forth out *.o cscope.out

check :
	rm -f out 
	./run-tests > out
	@echo "Diffs>"
	@diff expected out

