CC = arm-linux-gnueabi-gcc
BINS = hello ast sum

all : $(BINS)

hello : hello.S
	$(CC) -static $^ -o $@

ast : ast.S
	$(CC) -static $^ -o $@

sum : sum.S
	$(CC) -static $^ -o $@

clean :
	rm -$(BINS)
