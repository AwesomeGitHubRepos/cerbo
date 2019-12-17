BINS = hello ast

all : $(BINS)

hello : hello.S
	arm-linux-gnueabi-gcc -static $^ -o $@

ast : ast.S
	arm-linux-gnueabi-gcc -static $^ -o $@
clean :
	rm -$(BINS)
