BUILT = lex.yy.cc yacc.tab.cc yacc.tab.h

blang : blang.cc lex.yy.cc yacc.tab.cc
	g++ -std=c++17 $^ -o $@

yacc.tab.cc : yacc.y
	bison -o $@ --defines=yacc.tab.h $<
	
lex.yy.cc : lex.l yacc.tab.cc
	flex  -o $@ -i $< 

clean :
	rm -f $(BUILT)
