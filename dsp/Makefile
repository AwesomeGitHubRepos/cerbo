#OBJS = hz440.o white.o
BINS = brown choco hz440 pink white
LIBS = -lm

all : $(BINS)

*.o : *.c
	gcc -c $^

brown : brown.c
	gcc $^ -o $@ -lm

pink : pink.cc
	g++ $^ -o $@

clean :
	rm -f *raw *.wav $(BINS)
