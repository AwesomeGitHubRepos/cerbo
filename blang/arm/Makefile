CC = arm-linux-gnueabi-gcc

BINS = test goto
BUILT = test.S goto.S

all : $(BINS)



goto : goto.S
	$(CC) -static $^ -o $@

goto.S : goto.bas blang.p6 template.asm
	perl6 blang.p6 < $< > $@

test : test.S
	$(CC) -static $^ -o $@

test.S : test.bas blang.p6 template.asm
	perl6 blang.p6 < $< > $@


clean:
	rm -f $(BINS) $(BUILT)

