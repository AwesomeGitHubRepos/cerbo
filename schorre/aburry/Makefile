metaii :  meta.cpp
	g++ -I. --whole-program  -g $^ -o $@

expr.cpp : expr.meta metaii 
	./metaii < expr.meta > expr.cpp

expr : expr.cpp
	g++ -I. --whole-program  -g $^ -o $@

clean :
	rm -f expr.cpp expr
