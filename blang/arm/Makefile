CC = arm-linux-gnueabi-gcc


test.S : test.bas blang.p6
	perl6 blang.p6 < test.bas > test.S

test : test.S
	$(CC) -static $^ -o $@

clean:
	rm -f test test.S

