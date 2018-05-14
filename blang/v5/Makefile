ASAN = -fsanitize=address

all: blang5

clean:
	rm -f blang5 lex.yy.cc

lex.yy.cc : blang.lex
	flex++ $^

blang5 : blang.cc lex.yy.cc
	g++ $(ASAN) $^ -o $@
