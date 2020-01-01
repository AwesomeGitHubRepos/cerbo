CC = arm-linux-gnueabi-gcc


test : test.S
	$(CC) -static $^ -o $@

test.S : test.bas blang.p6 template.asm
	perl6 blang.p6 < test.bas > test.S


clean:
	rm -f test test.S

