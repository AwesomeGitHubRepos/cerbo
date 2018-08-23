.PHONY: clean

#all : lex-out.scm calc-yy.scm
	
calc : calc.scm calc-yy.o lex-out.o
	csc $^

lex-out.scm : lex.l
	csi make-lex.scm

lex-out.o : lex-out.scm
	csc -unit lex-out -c $^

calc-yy.scm : calc.grm
	csi < calc.grm

calc-yy.o : calc-yy.scm
	csc -unit calc-yy -c $^


clean :
	rm -f lex-out.scm calc.yy.scm *.o *~ calc.out calc
