.PHONY: clean

#all : lex-out.scm calc-yy.scm
	
blangc : blangc.scm grammar-out.o lex-out.o
	csc $^

lex-out.scm : lex.l
	csi make-lex.scm

lex-out.o : lex-out.scm
	csc -unit lex-out -c $^

grammar-out.scm : grammar.grm
	csi < $^

grammar-out.o : grammar-out.scm
	csc -unit grammar-out -c $^


clean :
	rm -f lex-out.scm grammar-out.scm  grammar.out *.o *~  blangc
