.PHONY : all clean

all : calc

clean :
	rm -f calc lexer.ml parser.ml parser.mli *.cmi *.cmo


parser.ml parser.mli : parser.mly lexer.ml
	ocamlyacc parser.mly

parser.cmi : parser.mli
	ocamlc -c parser.mli

lexer.ml : lexer.mll parser.cmi
	ocamllex lexer.mll

lexer.cmi lexer.cmo : lexer.ml
	ocamlc -c lexer.ml

parser.cmo : parser.ml
	ocamlc -c parser.ml

calc.cmi calc.cmo : calc.ml
	ocamlc -c calc.ml

calc : lexer.cmo parser.cmo calc.cmo
	ocamlc -o calc lexer.cmo parser.cmo calc.cmo
