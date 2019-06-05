#OBJS = hz440.o white.o
BINS = brown choco hz440 white
LIBS = -lm

all : $(BINS)

*.o : *.c
	gcc -c $^

brown : brown.c
	gcc $^ -o $@ -lm

clean :
	rm -f *raw *.wav $(BINS)
