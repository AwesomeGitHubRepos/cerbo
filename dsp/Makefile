#OBJS = hz440.o white.o
BINS = brown choco db05-117 hz440 pink white
LIBS = -lm

all : $(BINS)

*.o : *.c
	gcc -c $^

brown : brown.c
	gcc $^ -o $@ -lm

hz440 : hz440.c
	gcc $^ -o $@ -lm

pink : pink.cc
	g++ $^ -o $@

db05-117 : db05-117.f90
	gfortran $^ -o $@

db05-117.out : db05-117
	./$^ > $@

plot : db05-117.out	
	#cat db05-117.out | gnuplot -p -e 'plot "/dev/stdin" with lines'
	./db05-117 >db05-117.out


white.wav : white
	./white
	sox -r 16k -e unsigned -b 8 -c 1 white.raw white.wav
    
clean :
	rm -f *raw *.wav $(BINS)
