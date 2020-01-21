metaii :  meta.cpp
	g++ -I. --whole-program  $^ -o $@

expr.cpp : expr.meta metaii 
	./metaii < expr.meta > expr.cpp

clean :
	rm -f expr.cpp
