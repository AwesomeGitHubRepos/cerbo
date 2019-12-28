MACHINE = $(shell uname -m)
ifeq ($(MACHINE),armv7l)
	CC = gcc
else
	CC = arm-linux-gnueabi-gcc
endif

BINS = hello ast sum parabola

all : $(BINS)

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
