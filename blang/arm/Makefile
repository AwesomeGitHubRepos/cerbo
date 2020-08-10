CC = arm-linux-gnueabi-gcc

BINS = test goto print
BUILT = test.S goto.S print.S

all : $(BINS)

print : print.S
	 $(CC) -static $^ -o $@

print.S : print.bas
	perl6 blang.p6 < $< > $@

#print : print.bas blang.p6 template.asm
#	perl6 blang.p6 < $< > asm.S
#	$(CC) -static asm.S -o $@

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

