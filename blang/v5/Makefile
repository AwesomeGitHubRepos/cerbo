ASAN = -fsanitize=address

all: blang5

clean:
	rm -f blang5 lex.yy.cc 	blang.tab.cc 	blang.tab.hh 	location.hh \
	position.hh 	stack.hh \
	Scanner.ih Scannerbase.h lex.cc parse.cc \
	lex.yy.c lex.yy.h




lex.yy.cc : blang.lex
	flex -+  $^

#lex.cc : lexer
#	flexc++ lexer

blang.tab.cc : blang.yy lex.yy.cc
	bison -d blang.yy

#parse.cc: grammar
#	bisonc++ grammar

#blang5 : blang.cc lex.cc parse.cc
blang5 : blang.cc lex.yy.cc blang.tab.cc
	g++ -Wno-register --std=c++17 $(ASAN) $^ -o $@
