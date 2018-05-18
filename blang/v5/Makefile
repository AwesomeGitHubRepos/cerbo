ASAN = -fsanitize=address

CXXFLAGS = -O0 -ggdb --std=c++17
OBJS = blang.o funcs.o blang.tab.o lex.yy.o

all: blang5

wc:
	wc -l blang.cc blang.lex blang.yy funcs.cc

clean:
	rm -f blang5 lex.yy.cc 	blang.tab.cc 	blang.tab.hh \
	lex.yy.c lex.yy.h \
	*.o

%.o : %.cc
	g++ $(CXXFLAGS)  -c $<

blang.tab.o : blang.yy
	bison -d blang.yy
	g++ $(CXXFLAGS) -c blang.tab.cc

lex.yy.o : blang.lex
	flex -+  blang.lex
	g++ $(CXXFLAGS) -Wno-register -c lex.yy.cc






blang5 : $(OBJS)
	g++  $(ASAN) $^ -o $@
