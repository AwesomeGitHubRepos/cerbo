#OBJS = hz440.o white.o
BINS = brown choco db05-117 hz440 pink white
LIBS = -lm

all : $(BINS)

*.o : *.c
	gcc -c $^

brown : brown.c
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


clean :
	rm -f *raw *.wav $(BINS)
