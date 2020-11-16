#CFLAGS = -ggdb -fsanitize-address
CFLAGS = -g -O0

CC = g++

all: forth

forth.o : forth.cpp
	$(CC) $(CFLAGS) -c $^ 

forth : forth.o
	$(CC) $^ -o $@

clean:
	rm -f forth out *.o

check :
	rm -f out 
	./run-tests > out
	@echo "Diffs>"
	@diff expected out

