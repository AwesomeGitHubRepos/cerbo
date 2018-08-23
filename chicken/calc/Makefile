.PHONY: clean

all : lex-out.scm calc.yy.scm
	
lex-out.scm : lex.l
	csi make-lex.scm

calc.yy.scm : calc.grm
	csi < calc.grm

clean :
	rm -f lex-out.scm calc.yy.scm
