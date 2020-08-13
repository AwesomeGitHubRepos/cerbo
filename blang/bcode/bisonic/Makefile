BUILT = lex.yy.cc

blang : blang.cc lex.yy.cc
	g++ -std=c++17 $^ -o $@

lex.yy.cc : lex.l
	flex  -o $@ -i $< 

clean :
	rm -f $(BUILT)
