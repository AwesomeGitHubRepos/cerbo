ASAN = -fsanitize=address

CXXFLAGS = -O0 -ggdb --std=c++17
OBJS = blang.o blang.tab.o lex.yy.o

all: blang5

wc:
	wc -l blang.h blang.cc blang.lex blang.yy 

clean:
	rm -f blang5 lex.yy.cc 	blang.tab.cc blang.output blang.tab.hh \
	lex.yy.c lex.yy.h \
	*.o

%.o : %.cc blang.h
	g++ $(CXXFLAGS)  -c $<

blang.tab.o : blang.yy blang.h
	bison -v -d blang.yy
	g++ $(CXXFLAGS) -c blang.tab.cc

blang.cc : blang.tab.o

lex.yy.o : blang.lex blang.h
	flex -+  blang.lex
	g++ $(CXXFLAGS) -Wno-register -c lex.yy.cc






blang5 : $(OBJS)
	g++  $(ASAN) $^ -o $@
