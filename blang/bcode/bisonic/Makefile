CXXFLAGS = -std=c++17

BUILT = lex.yy.cc yacc.tab.cc yacc.tab.h

blang : main.o lex.yy.cc yacc.tab.cc
	g++ $(CXXFLAGS) $^ -o $@

main.o : main.cc
	g++ $(CXXFLAGS) -c $<

yacc.tab.cc : yacc.y
	bison -o $@ --defines=yacc.tab.h $<
	
lex.yy.cc : lex.l yacc.tab.cc
	flex  -o $@ -i $< 

clean :
	rm -f $(BUILT)
