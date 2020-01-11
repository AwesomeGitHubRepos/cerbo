MACHINE = $(shell uname -m)
ifeq ($(MACHINE),armv7l)
	CC = gcc
else
	CC = arm-linux-gnueabi-gcc
endif

BINS = catasm hello ast sum parabola mean

all : $(BINS)

mean : mean.s
	$(CC) -mfpu=vfp -static $^ -o $@

catasm : catasm.s
	$(CC) -static $^ -o $@

parabola : parabola.S
	$(CC) -static $^ -o $@

hello : hello.S
	echo "v$(MACHINE)n"
	$(CC) -static $^ -o $@

ast : ast.S
	$(CC) -static $^ -o $@

sum : sum.S
	$(CC) -static $^ -o $@

clean :
	rm -$(BINS)
